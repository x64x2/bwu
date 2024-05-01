package page

import (
	"io"
)

//Parser is an interface which parse the given HTML page as input IO reader and returns a set of links found
type Parser interface {
	Parse(input io.Reader) []string
}
