#include "BitmapFont.hpp"
#include "Globals.hpp"

BitmapFont::BitmapFont( const std::string& imageFileName, int charWidth, int charHeight ) :
	image(imageFileName),
	charWidth(charWidth),
	charHeight(charHeight)
{
	charData.resize(94, Vector2<int>(-1, -1));
}

void BitmapFont::addCharacter( char ch, int x, int y )
{
	charData[ch - ' ' - 1] = Vector2<int>(x, y);
}

void BitmapFont::addCharacters( char ch, int x, int y, int number )
{
	for( int i = 0; i < number; i++ )
	{
		addCharacter( ch + (char)i, x + charWidth * i, y );
	}
}

int BitmapFont::getCharacterHeight() const
{
	return charHeight;
}

int BitmapFont::getCharacterWidth() const
{
	return charWidth;
}

int BitmapFont::getTextWidth( const std::string& text ) const
{
	return getCharacterWidth() * text.length();
}


