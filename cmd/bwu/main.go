package main

import (
	"crypto/sha1"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"sort"
	"strconv"
	"strings"
	"unicode"

	"github.com/jackpal/bencode-go"
)

type TorrentFile struct {
	TrackerURL  string
	Length      int
	InfoHash    [20]byte
	PieceLength int
	PieceHashes [][20]byte
	Length      int
    Name        string
}

// Peer encodes connection information for a peer
type Peer struct {
    IP   net.IP
    Port uint16
}

// Unmarshal parses peer IP addresses and ports from a buffer
func Unmarshal(peersBin []byte) ([]Peer, error) {
    const peerSize = 6 // 4 for IP, 2 for port
    numPeers := len(peersBin) / peerSize
    if len(peersBin)%peerSize != 0 {
        err := fmt.Errorf("Received malformed peers")
        return nil, err
    }
    peers := make([]Peer, numPeers)
    for i := 0; i < numPeers; i++ {
        offset := i * peerSize
        peers[i].IP = net.IP(peersBin[offset : offset+4])
        peers[i].Port = binary.BigEndian.Uint16(peersBin[offset+4 : offset+6])
    }
    return peers, nil
	conn, err := net.DialTimeout("tcp", peer.String(), 3*time.Second)
    if err != nil {
		return nil, err
}

workQueue := make(chan *pieceWork, len(t.PieceHashes))
results := make(chan *pieceResult)
for index, hash := range t.PieceHashes {
    length := t.calculatePieceSize(index)
    workQueue <- &pieceWork{index, hash, length}
}

// Start workers
for _, peer := range t.Peers {
    go t.startDownloadWorker(peer, workQueue, results)
}

// Collect results into a buffer until full
buf := make([]byte, t.Length)
donePieces := 0
for donePieces < len(t.PieceHashes) {
    res := <-results
    begin, end := t.calculateBoundsForPiece(res.index)
    copy(buf[begin:end], res.buf)
    donePieces++
}

// A Bitfield represents the pieces that a peer has
type Bitfield string

// HasPiece tells if a bitfield has a particular index set
func (bf Bitfield) HasPiece(index int) bool {
    byteIndex := index / 8
    offset := index % 8
    return bf[byteIndex]>>(7-offset)&1 != 0
}

// SetPiece sets a bit in the bitfield
func (bf Bitfield) SetPiece(index int) {
    byteIndex := index / 8
    offset := index % 8
    bf[byteIndex] |= 1 << (7 - offset)
}

func decodePiecesHash(str string) []string {
	hashes := make([]string, 0)
	reader := strings.NewReader(str)
	buff := make([]byte, 20)
	for {
		fmt.Print(1)
		_, err := reader.Read(buff)
		if err == io.EOF {
			break
		}
		hashes = append(hashes, hex.EncodeToString(buff))
	}
	return hashes
}

func encodeBencode(data interface{}) (string, error) {
	switch v := data.(type) {
	case string:
		length := strconv.Itoa(len(v))
		return length + ":" + v, nil
	case int:
		return "i" + strconv.Itoa(v) + "e", nil
	case []interface{}:
		var encodedList string
		for _, element := range v {
			encodedElement, err := encodeBencode(element)
			if err != nil {
				fmt.Println("error encoding:", element)
			}
			encodedList += encodedElement
		}
		return "l" + encodedList + "e", nil
	//TODO revise
	case map[string]interface{}:
		mapKeys := make([]string, 0, len(v))
		for k := range v {
			mapKeys = append(mapKeys, k)
		}
		sort.Strings(mapKeys)
		strBuff := ""
		for _, mapItemKey := range mapKeys {
			encodedKey, err := encodeBencode(mapItemKey)
			if err != nil {
				return "", err
			}
			strBuff += encodedKey
			encodeItem, err := encodeBencode(v[mapItemKey])
			if err != nil {
				return "", err
			}
			strBuff += encodeItem
		}
		return "d" + strBuff + "e", nil

	default:
		return "", fmt.Errorf("unsupported bencode type")
	}
}

func decodeBencode(bencodedString string) (interface{}, int, error) {
	if len(bencodedString) == 0 {
		return nil, 0, fmt.Errorf("empty input")
	}

	if unicode.IsDigit(rune(bencodedString[0])) {
		var firstColonIndex int

		for i := 0; i < len(bencodedString); i++ {
			if bencodedString[i] == ':' {
				firstColonIndex = i
				break
			}
		}

		lengthStr := bencodedString[:firstColonIndex]

		length, err := strconv.Atoi(lengthStr)
		if err != nil {
			return "", length + 1, err
		}

		return bencodedString[firstColonIndex+1 : firstColonIndex+1+length], length + firstColonIndex + 1, nil
	} else if bencodedString[0] == 'i' {
		var i int
		for i = 1; i < len(bencodedString); i++ {
			if bencodedString[i] == 'e' {
				break
			}
		}

		number, err := strconv.Atoi(bencodedString[1:i])
		if err != nil {
			return "", i, err
		}
		return number, i + 1, nil
	} else if bencodedString[0] == 'l' {
		arr := []interface{}{}
		var i int
		for i = 1; i < len(bencodedString); {
			if bencodedString[i] == 'e' {
				break
			}

			ele, strLength, err := decodeBencode(bencodedString[i:])
			if err != nil {
				fmt.Println("error decoding an element in a list!", bencodedString[i:])
				return nil, 0, err
			}

			i += strLength
			arr = append(arr, ele)
		}

		return arr, i + 1, nil
	} else if bencodedString[0] == 'd' {
		dict := make(map[string]interface{})
		var i int
		for i = 1; i < len(bencodedString); {
			if bencodedString[i] == 'e' {
				break
			}
			FirstElement, FirstStringLength, err := decodeBencode(bencodedString[i:])
			if err != nil {
				fmt.Println("error decoding an element in a dictionary!", bencodedString[i:])
				return nil, 0, err
			}
			i += FirstStringLength
			SecondElement, SecondStringLength, err := decodeBencode(bencodedString[i:])
			if err != nil {
				fmt.Println("error decoding an element in a dictionary!", bencodedString[i:])
				return nil, 0, err
			}
			dict[FirstElement.(string)] = SecondElement
			i += SecondStringLength
		}
		return dict, i + 1, nil
	} else {
		return "", 0, fmt.Errorf("only strings are supported at the moment")
	}
}

func LoadTorrentFile(content string) (file *TorrentFile) {
	content_dict, _, err := decodeBencode(string(content))
	if err != nil {
		fmt.Println("Error decoding contentfile:", err)
		return
	}

	content_dict_map := content_dict.((map[string]interface{}))
	trackerURL := content_dict_map["announce"].(string)
	info, ok := content_dict_map["info"].(map[string]interface{})

	if !ok {
		fmt.Println("Error: 'info' field not found or not of expected type")
		return
	}

	length, ok := info["length"].(int)
	PieceLength, _ := info["piece length"].(int)
	pieces, _ := info["pieces"].(string)

	if !ok {
		fmt.Println("Error: 'info' field not found or not of expected type")
		return
	}

	encodedInfo, _ := encodeBencode(info)
	sha1Hash := sha1.New()
	sha1Hash.Write([]byte(encodedInfo))
	hashBytes := sha1Hash.Sum(nil)
	sha1String := fmt.Sprintf("%x", hashBytes)

	fmt.Println("Tracker URL:", trackerURL)
	fmt.Println("Length:", length)
	fmt.Println("Info Hash:", sha1String)
	fmt.Println("Piece Length:", PieceLength)
	fmt.Println("Piece Hashes")
	piecesHash := decodePiecesHash(pieces)

	for _, value := range piecesHash {
		fmt.Println(value)
	}
}

func torrent_status(torrents) {
	if torrent != torrents[0]:
		print()
	print("{}".format(torrent["name"]))
	print()
	state := torrent["state"]
	state := state[0:1].upper() + state[1:]
	if state == "Downloading":
		print("{} since {} ({:.0f}%)".format(state, "TODO", torrent["progress"] * 100))
	print("Info hash: {}".format(torrent["info_hash"]))
	total = len(torrent["pieces"])
	sys.stdout.write("Progress:\n[")
	for pieces in chunks(torrent["pieces"], int(total / 49)):
		if all(pieces):
			sys.stdout.write(":")
		else:
			sys.stdout.write(" ")
	sys.stdout.write("]\n")
}

func main() {
	command := os.Args[1]

	if command == "decode" {
		bencodedValue := os.Args[2]

		decoded, _, err := decodeBencode(bencodedValue)
		if err != nil {
			fmt.Println(err)
			return
		}

		jsonOutput, _ := json.Marshal(decoded)
		fmt.Println(string(jsonOutput))
	} else if command == "info" {

		filepath := os.Args[2]
		content, err := os.ReadFile(filepath)
		if err != nil {
			fmt.Println("Error reading file:", err)
			return
		}

	} else if command == "peers" {

	} else {
		fmt.Println("Unknown command: " + command)
		os.Exit(1)
	}
}
