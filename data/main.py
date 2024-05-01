import argparse
import json
import logging
import requests
import time
from datetime import datetime, timedelta
from pathlib import Path

import requests


class requester:
    def __init__(
        self,
        monitor: bool,
        run_in_docker: bool,
        topic: list = None,
        exclude_topic: bool = False,
        request_time_limit: float = 1,
        stream_log_level=logging.INFO,
        logfolderpath: str = "logs",
    ):
        if run_in_docker:
            self._base_save_path: Path = Path("/data")
        else:
            self._base_save_path: Path = Path().resolve() / "data"
        self._save_debuglog = True
        self._stream_log_level = stream_log_level
        self._setup_logging(logfolderpath)

        self.monitor: bool = monitor
        self._include_topic: list = topic
        self._exclude_topic: bool = exclude_topic
        self._request_time_limit: float = request_time_limit
        self._check_new_topic: bool = True
        self._last_requested = {}

        self.last_request = None

        if self.monitor is True:
            self.begin_monitoring()

    def begin_monitoring(self):
        self._logger.info("Beginning monitoring")
        self._logger.info(f"Storing data in path: {self._base_save_path}")
        self.monitor = True
        self.monitoring_topic = []
        self.monitoring_posts = {}
        self._posts_last_checked = {}
        self._logger.debug("Initialising Monitoring post")
        self._monitor_post = posting.post(target=self._begin_monitoring)
        self._logger.debug("Starting post")
        self._monitor_post.start()
        self._logger.debug("Monitoring Started")

    def end_monitoring(self):
        self._logger.info("Ending loop and closing monitoring post")
        self.monitor = False
        self._monitor_post.join()
        self._logger.info("Closed monitoring post")

    def _load_old_monitors(self):
        self._update_monitoring_topic()
        self._logger.debug(
            "Checking for past captures of old posts in previous instances"
        )
        old_monitor_dict = {}
        old_posts = 0
        for topic in self.monitoring_topic:
            timestamp = self._get_day()
            topicfolderpath = (
                self._base_save_path / "saves" / timestamp / "posts_on_topic"
            )
            topicfolderpath.mkdir(parents=True, exist_ok=True)
            postpath = self._base_save_path / "saves" / timestamp / "posts" / topic
            postpath.mkdir(parents=True, exist_ok=True)

            good_topicpath_found = False
            for topicpath in topicfolderpath.iterdir():
                if topicpath.name.split("_")[0] == str(topic):
                    good_topicpath = topicpath
                    good_topicpath_found = True

            if not good_topicpath_found:
                self._logger.info(
                    f"No previous post information for /{topic}/, no old posts to monitor"
                )
                continue
            old_monitor_dict[topic] = {}

            with open(good_topicpath, "r") as prev_posts_file:
                prev_posts = json.load(prev_posts_file)
                for page in prev_posts:
                    for posts in page["posts"]:
                        old_monitor_dict[str(topic)][str(posts["no"])] = [
                            int(posts["last_modified"]),
                            int(posts["replies"]),
                        ]
                        old_posts += 1
            self._logger.debug(
                f"{len} past captures of old posts in previous instances of {topic} discovered"
            )

        self.monitoring_posts = old_monitor_dict
        self._logger.debug(
            f"{old_posts} past captures of old posts in previous instances discovered"
        )

    def set_include_exclude_topic(
        self, include_topic: list = None, exclude_topic: bool = False
    ):
        self._logger.info("Updating topic to monitor")
        self._include_topic = include_topic
        self._exclude_topic = exclude_topic
        if include_topic is None and not exclude_topic:
            self._check_new_topic = False
        else:
            self._check_new_topic = True

    def _update_monitoring_topic(self):
        self._logger.debug("Updating monitor topic list (checking)")
        if self._include_topic is not None and not self._exclude_topic:
            self.monitoring_topic = self._include_topic
        elif self._include_topic is not None and self._exclude_topic:
            self.monitoring_topic = list(
                set(self._set_topic_list()).difference(self._include_topic)
            )
        else:
            self.monitoring_topic = self._set_topic_list()
        self._check_new_topic = False

    def _begin_monitoring(self):
        self._logger.debug("_begin_monitoring entered")
        self._load_old_monitors()
        self._logger.debug("Old monitors retrieved")
        while self.monitor is True:
            self._logger.debug("Started loop")
            if self._check_new_topic:
                self._logger.debug("Started updating monitoring topic")
                self._update_monitoring_topic()
            self._update_monitoring_posts()
            self._logger.debug("updating posts on monitoring list")
            self._update_posts_on_monitoring_postlist()
            self._logger.debug("Ended loop")

    def _update_monitoring_posts(self):
        self._logger.info("Beginning search for posts to monitor")
        self._posts_to_update = []
        death_count = 0
        birth_count = 0
        update_count = 0
        for topic in self.monitoring_topic:
            self._logger.info(f"Searching for posts in {topic}")
            posts_json = self.get_and_save_single_topic_postlist(
                topic, with_return=True
            )
            if posts_json is None:
                continue
            posts_on_topic = {}
            for page in posts_json:
                for post in page["posts"]:
                    posts_on_topic[str(post["no"])] = [
                        int(post["last_modified"]),
                        int(post["replies"]),
                    ]

            if topic in self.monitoring_posts:
                for post in self.monitoring_posts[topic]:
                    if post in posts_on_topic:
                        pass
                    else:
                        self._logger.debug(f"post died: /{topic}/{post}")
                        death_count += 1
                        if post in self._last_requested[topic]["posts"]:
                            del self._last_requested[topic]["posts"][post]

                for post in posts_on_topic:
                    if post in self.monitoring_posts[topic]:
                        if (
                            self.monitoring_posts[topic][post][0]
                            < posts_on_topic[post][0]
                        ):
                            self._logger.debug(f"post updated: /{topic}/{post}")
                            self.monitoring_posts[topic][post] = posts_on_topic[
                                post
                            ]
                            self._posts_to_update.append([topic, post])
                            update_count += 1
                        else:
                            self._logger.debug(
                                f"Do not need to update post /{topic}/{post}"
                            )
                    else:
                        self._logger.debug(f"New post: /{topic}/{post}")
                        self.monitoring_posts[topic][post] = posts_on_topic[
                            post
                        ]
                        self._posts_to_update.append([topic, post])
                        birth_count += 1
            else:
                self._logger.debug(f"New topic: updated to monitor list {topic}")
                self.monitoring_posts[topic] = posts_on_topic
                for post in posts_on_topic:
                    self._logger.debug(f"New post: /{topic}/{post}")
                    self._posts_to_update.append([topic, post])
                    birth_count += 1

        self._logger.info(f"post deaths in previous iteration: {death_count}")
        self._logger.info(f"post births in previous iteration: {birth_count}")
        self._logger.info(f"post updates in previous iteration: {update_count}")
        self._logger.info(
            f"{len(self.monitoring_posts[topic])} posts found to monitor."
        )

    def _update_posts_on_monitoring_postlist(self):
        number_posts_in_iteration = len(self._posts_to_update)
        i = 1
        prev_topic = ""
        for topic, post in self._posts_to_update:
            if prev_topic != topic:
                self._logger.info(f"Updating posts in {topic}")
                prev_topic = topic
            start_time = time.time()
            self.get_and_save_post(topic, post)
            current_time_diff = (time.time() - start_time) * (
                number_posts_in_iteration - i
            )
            self._logger.debug(
                f"{i}/{number_posts_in_iteration}: Capturing post {post} in /{topic}/ approximate seconds remaining in iteration {current_time_diff:n}"
            )
            i += 1

    def _set_topic_list(self):
        topic_info = self.get_chan_info_json()
        codes = [topic["topic"] for topic in topic_info["topic"]]
        return codes

    @staticmethod
    def _get_time():
        now = datetime.utcnow()
        return now.strftime("_%H_%M_%S")

    @staticmethod
    def _get_day():
        now = datetime.utcnow()
        return now.strftime("%Y_%m_%d")

    @staticmethod
    def _get_full_time():
        now = datetime.utcnow()
        return now.strftime("%Y_%m_%d_%H_%M_%S")

    def _check_time_and_wait(self):
        if self.last_request is None:
            self.last_request = time.time()
        else:
            if time.time() - self.last_request >= self._request_time_limit:
                self.last_request = time.time()
            else:
                time.sleep(self._request_time_limit - (time.time() - self.last_request))
                self.last_request = time.time()

    def get_chan_info_json(self):  #req ifo
        self._check_time_and_wait()
        self._logger.debug("chan information requested")
        r_topic = requests.get("http://nairaland.com/topic.json")
        return r_topic.json()

    @staticmethod
    def _format_if_mod_since_header(since):
        since = time.gmtime(time.mktime(since))
        return {"If-Modified-Since": time.strftime("%a, %d %b %Y %H:%M:%S GMT", since)}

    def get_single_topic_postlist(self, topic_code: str):
        self._check_time_and_wait()
        self._logger.debug(f"topic /{topic_code}/ post information requested")
        if topic_code not in self._last_requested:
            self._last_requested[topic_code] = {}
            self._last_requested[topic_code]["posts"] = {}
            r_post_list = requests.get(
                "https://nairaland.com/" + topic_code + "/posts.json"
            )
        else:
            topic_request_time = datetime.now() - datetime.fromtimestamp(
                time.mktime(self._last_requested[topic_code]["topic"])
            )
            if topic_request_time < timedelta(seconds=10):
                sleeping = 10 - topic_request_time.total_seconds()
                self._logger.info(
                    f"Sleeping for {sleeping} seconds: time between requests for posts on topic {topic_code} too short"
                )
                time.sleep(sleeping)
            r_post_list = requests.get(
                "https://nairaland.com/" + topic_code + "/posts.json",
                headers=self._format_if_mod_since_header(
                    self._last_requested[topic_code]["topic"]
                ),
            )

        self._last_requested[topic_code]["topic"] = datetime.now().timetuple()
        if r_post_list.status_code == 304:
            self._logger.info(f"No new posts on topic /{topic_code}/")
            return None
        return r_post_list.json()

    def get_post(self, topic_code: str, op_ID: int):
        self._check_time_and_wait()
        if topic_code not in self._last_requested[topic_code]["posts"]:
            r_post = requests.get(
                "https://nairaland.com/" + topic_code + "/post/" + str(op_ID) + ".json"
            )
        else:
            r_post = requests.get(
                "https://nairaland.com/" + topic_code + "/post/" + str(op_ID) + ".json",
                headers=self._format_if_mod_since_header(
                    self._last_requested[topic_code]["posts"][op_ID]
                ),
            )
        self._last_requested[topic_code]["posts"][op_ID] = datetime.now().timetuple()

        countdown = 1
        while r_post.status_code not in [200, 304]:
            if r_post.status_code == 404:
                self._logger.warning(
                    f"Request for post {op_ID} on topic /{topic_code}/ was unsuccessful with error code {r_post.status_code}, skipping"
                )
                return None
            elif countdown < 6:
                self._logger.error(
                    f"Request for post {op_ID} on topic /{topic_code}/ was unsuccessful with error code {r_post.status_code}, trying {countdown} more times"
                )
            else:
                self._logger.warning(
                    f"Request for post {op_ID} on topic /{topic_code}/ was unsuccessful with error code {r_post.status_code}, returning None"
                )
                return None
            time.sleep(self._request_time_limit * 5)
            r_post = requests.get(
                "https://nairaland.com/" + topic_code + "/post/" + str(op_ID) + ".json",
                headers=self._format_if_mod_since_header(
                    self._last_requested[topic_code]["posts"][op_ID]
                ),
            )
            self._last_requested[topic_code]["posts"][
                op_ID
            ] = datetime.now().timetuple()
            countdown += 1
        if r_post.status_code == 304:
            self._logger.debug(f"post {op_ID} not updated since last request")
            return None
        elif r_post.status_code == 200:
            self._logger.debug("Recieved answer")
        time.sleep(self._request_time_limit)
        return r_post.json()

    def get_and_save_chan_info(self, outpath: Path = None, filename: str = None):
        timestamp = self._get_day()
        if outpath is None:
            outpath = self._base_save_path / "saves" / timestamp
        if filename is None:
            filename = "topic.json"
        outpath.mkdir(parents=True, exist_ok=True)
        with open(outpath / filename, "w") as outfile:
            json.dump(self.get_chan_info_json(), outfile, indent=2)

    def get_and_save_single_topic_postlist(
        self,
        topic_code: str,
        outpath: Path = None,
        filename: str = None,
        with_return: bool = False,
    ):
        timestamp = self._get_day()
        if outpath is None:
            outpath = self._base_save_path / "saves" / timestamp / "posts_on_topic"
        if filename is None:
            filename = topic_code + self._get_time() + ".json"
        outpath.mkdir(parents=True, exist_ok=True)
        postlist = self.get_single_topic_postlist(topic_code)
        if postlist is None:
            return None
        for posts in outpath.iterdir():
            if posts.name.split("_")[0] == str(topic_code):
                posts.unlink()
        with open(outpath / filename, "w") as outfile:
            json.dump(postlist, outfile, indent=2)
        if with_return:
            return postlist

    def get_and_save_post(
        self, topic_code: str, op_ID: int, outpath: Path = None, filename: str = None
    ):
        timestamp = self._get_day()
        if outpath is None:
            outpath = (
                self._base_save_path / "saves" / timestamp / "posts" / topic_code
            )
        outpath.mkdir(parents=True, exist_ok=True)

        if filename is None:
            filename = str(op_ID) + self._get_time() + ".json"
        fullname = outpath / filename
        new_post = True
        for posts in outpath.iterdir():
            if int(posts.name.split("_")[0]) == int(op_ID):
                with open(posts, "r+") as outfile:
                    try:
                        data = json.load(outfile)
                    except json.decoder.JSONDecodeError as jerror:
                        self._logger.warning(
                            f"Loading JSON file {posts} caused error {jerror}, continuing to writing new file rather than append. topic {topic_code}, post {op_ID}"
                        )
                        continue
                    else:
                        to_update = self.get_post(topic_code, op_ID)
                        if to_update is None:
                            self._logger.warning(
                                f"Likely 404 caused no return for, skipping | topic {topic_code}, post {op_ID}"
                            )
                            new_post = False
                            continue
                        data.update(to_update)
                        outfile.seek(0)
                        json.dump(data, outfile, indent=2)
                        new_post = False
        if new_post is True:
            with open(fullname, "w") as outfile:
                json.dump(self.get_post(topic_code, op_ID), outfile, indent=2)

    def _setup_logging(self, logfolderpath: Path):
        logfolder = self._base_save_path / logfolderpath
        logfolder.mkdir(parents=True, exist_ok=True)

        self._logger = logging.getLogger("Nairaland_requester")
        self._logger.setLevel(logging.DEBUG)
        self._log_formatter = logging.Formatter(
            "%(asctime)s - %(name)s - %(levelname)s: %(postName)s - %(message)s"
        )

        self._streamlogs = logging.StreamHandler()
        self._streamlogs.setLevel(self._stream_log_level)
        self._streamlogs.setFormatter(self._log_formatter)
        self._logger.addHandler(self._streamlogs)

        self._infologpath = logfolder / ("info_log" + self._get_full_time() + ".log")
        self._infologfile = logging.FileHandler(self._infologpath)
        self._infologfile.setLevel(logging.INFO)
        self._infologfile.setFormatter(self._log_formatter)
        self._logger.addHandler(self._infologfile)

        if self._save_debuglog:
            self._debuglogpath = logfolder / (
                "debug_log" + self._get_full_time() + ".log"
            )
            self._debuglogfile = logging.FileHandler(self._debuglogpath)
            self._debuglogfile.setLevel(logging.DEBUG)
            self._debuglogfile.setFormatter(self._log_formatter)
            self._logger.addHandler(self._debuglogfile)

        self._logger.debug("Logger Initalised")


if __name__ == "__main__":
    argparser = argparse.ArgumentParser(description="4TCT tool")
    argparser.add_argument(
        "-d", action="store_true", help="Configure for running in docker"
    )
    argparser.add_argument(
        "-b",
        "--topic",
        metavar="topic:",
        nargs="*",
        action="store",
        type=str,
        default=None,
        help="List topic to include after this flag, use the short form topic name from 4chan, e.g. '-b a c g sci' would collect data from the topic /a/, /c/, /g/ and /sci/",
    )
    argparser.add_argument(
        "-e",
        "--exclude",
        action="store_true",
        help="Boolean flag - whether to exclude the flags after -b, e.g. '-b a c g sci -e' would exclude the topic /a/ /c/ /g/ and /sci/ from collection",
    )
    args = argparser.parse_args()
    requester_instance = requester(
        True, args.d, topic=args.topic, exclude_topic=args.exclude
    )
