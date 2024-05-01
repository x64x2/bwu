package sitemap

import (
	"sync"
	"sync/atomic"
)

// CoresTreshold defines a treshold to prefer sync.Map over map with RWMutex
const CoresTreshold = 4

//NewInMem accepts a number of available execution cores an returns best fit in memory Backend
// There are two available options to support in memory map backend.
// The first one is a regular map which is protected by a RWMutex, the second is new since Go 1.9+ sync.Map structure.
// The sync.Map has good performance benefit in case of number of execution cores at least 4
// runtime.NumCPU returns the number of logical CPUs usable by the current process.
func NewInMem(cores int) Backend {
	if cores < CoresTreshold {
		return &mutexMap{
			m: make(map[string]uint),
		}
	}
	return &syncedMap{}
}

//mutexMap implements store backend as in memory storage protected by a RWMutex
type mutexMap struct {
	sync.RWMutex
	m map[string]uint
}

func (m *mutexMap) Next() ([]string, bool) {
	m.RLock()
	defer m.RUnlock()
	response := make([]string, 0, len(m.m))
	for url := range m.m {
		//TODO maybe process links counter
		response = append(response, url)
	}
	return response, false
}

func (m *mutexMap) Append(url string) bool {
	m.Lock()
	defer m.Unlock()
	_, exists := m.m[url]
	m.m[url]++
	return !exists
}

func (m *mutexMap) Size() uint64 {
	m.RLock()
	defer m.RUnlock()
	return uint64(len(m.m))
}

//syncedMap implements store backend  which is based on sync.Map with atomic counter of stored records
type syncedMap struct {
	counter uint64
	m       sync.Map
}

func (s *syncedMap) Append(url string) bool {
	_, exists := s.m.LoadOrStore(url, nil)
	if !exists {
		atomic.AddUint64(&s.counter, 1)
	}
	return !exists
}

func (s *syncedMap) Size() uint64 {
	val := atomic.LoadUint64(&s.counter)
	return val
}

func (s *syncedMap) Next() ([]string, bool) {
	size := atomic.LoadUint64(&s.counter)
	resp := make([]string, 0, size) //preallocate memory once
	s.m.Range(func(key, value interface{}) bool {
		url, ok := key.(string)
		if ok {
			resp = append(resp, url)
		}
		return true
	})
	return resp, false
}
