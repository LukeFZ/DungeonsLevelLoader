#pragma once
#include "unreal.h"

struct FFontImportOptionsData
{

	/** Name of the typeface for the font to import */
	FString FontName;

	/** Height of font (point size) */
	float Height;    

	/** Whether the font should be antialiased or not.  Usually you should leave this enabled. */
	uint32 bEnableAntialiasing:1;

	/** Whether the font should be generated in bold or not */
	uint32 bEnableBold:1;

	/** Whether the font should be generated in italics or not */
	uint32 bEnableItalic:1;

	/** Whether the font should be generated with an underline or not */
	uint32 bEnableUnderline:1;

	/** if true then forces PF_G8 and only maintains Alpha value and discards color */
	uint32 bAlphaOnly:1;

	/** Character set for this font */
	//TEnumAsByte<enum EFontImportCharacterSet> CharacterSet;
	uint8 CharacterSet;

	/** Explicit list of characters to include in the font */
	FString Chars;

	/** Range of Unicode character values to include in the font.  You can specify ranges using hyphens and/or commas (e.g. '400-900') */
	FString UnicodeRange;

	/** Path on disk to a folder where files that contain a list of characters to include in the font */
	FString CharsFilePath;

	/** File mask wildcard that specifies which files within the CharsFilePath to scan for characters in include in the font */
	FString CharsFileWildcard;

	/** Skips generation of glyphs for any characters that are not considered 'printable' */
	uint32 bCreatePrintableOnly:1;

	/** When specifying a range of characters and this is enabled, forces ASCII characters (0 thru 255) to be included as well */
	uint32 bIncludeASCIIRange:1;

	/** Color of the foreground font pixels.  Usually you should leave this white and instead use the UI Styles editor to change the color of the font on the fly */
	FLinearColor ForegroundColor;

	/** Enables a very simple, 1-pixel, black colored drop shadow for the generated font */
	uint32 bEnableDropShadow:1;

	/** Horizontal size of each texture page for this font in pixels */
	int32 TexturePageWidth;

	/** The maximum vertical size of a texture page for this font in pixels.  The actual height of a texture page may be less than this if the font can fit within a smaller sized texture page. */
	int32 TexturePageMaxHeight;

	/** Horizontal padding between each font character on the texture page in pixels */
	int32 XPadding;

	/** Vertical padding between each font character on the texture page in pixels */
	int32 YPadding;

	/** How much to extend the top of the UV coordinate rectangle for each character in pixels */
	int32 ExtendBoxTop;

	/** How much to extend the bottom of the UV coordinate rectangle for each character in pixels */
	int32 ExtendBoxBottom;

	/** How much to extend the right of the UV coordinate rectangle for each character in pixels */
	int32 ExtendBoxRight;

	/** How much to extend the left of the UV coordinate rectangle for each character in pixels */
	int32 ExtendBoxLeft;

	/** Enables legacy font import mode.  This results in lower quality antialiasing and larger glyph bounds, but may be useful when debugging problems */
	uint32 bEnableLegacyMode:1;

	/** The initial horizontal spacing adjustment between rendered characters.  This setting will be copied directly into the generated Font object's properties. */
	int32 Kerning;

	/** If true then the alpha channel of the font textures will store a distance field instead of a color mask */
	uint32 bUseDistanceFieldAlpha:1;

	/** 
	* Scale factor determines how big to scale the font bitmap during import when generating distance field values 
	* Note that higher values give better quality but importing will take much longer.
	*/
	int32 DistanceFieldScaleFactor;

	/** Shrinks or expands the scan radius used to determine the silhouette of the font edges. */
	float DistanceFieldScanRadiusScale;

};

struct FFontData {
		/**
	 * The filename of the font to use.
	 * This variable is ignored if we have a font face asset, and is set to the .ufont file in a cooked build.
	 */
	FString FontFilename;

	/**
	 * The hinting algorithm to use with the font.
	 * This variable is ignored if we have a font face asset, and is synchronized with the font face asset on load in a cooked build.
	 */
//	EFontHinting Hinting;
	uint8 Hinting;

	/**
	 * Enum controlling how this font should be loaded at runtime. See the enum for more explanations of the options.
	 * This variable is ignored if we have a font face asset, and is synchronized with the font face asset on load in a cooked build.
	 */
//	EFontLoadingPolicy LoadingPolicy;
	uint8 LoadingPolicy;

	/**
	 * The index of the sub-face that should be used.
	 * This is typically zero unless using a TTC/OTC font.
	 */
	int32 SubFaceIndex;

	/**
	 * Font data v3. This points to a font face asset.
	 */
	const UObject* FontFaceAsset;
};

struct FTypefaceEntry {
	/** Name used to identify this font within its typeface */
	FName Name;

	/** Raw font data for this font */
	FFontData Font;
};
struct FTypeface {
		TArray<FTypefaceEntry> Fonts;
};
struct FCompositeFallbackFont {
	/** Typeface data for this sub-font */
	FTypeface Typeface;

	/** Amount to scale this sub-font so that it better matches the size of the default font */
	float ScalingFactor;
};
struct FCompositeSubFont {
	FTypeface DefaultTypeface;

	/** The fallback typeface that will be used as a last resort when no other typeface provides a match */
	FCompositeFallbackFont FallbackTypeface;

	/** Sub-typefaces to use for a specific set of characters */
	TArray<FCompositeSubFont> SubTypefaces;
};

struct FCompositeFont {
	FTypeface DefaultTypeface;

	FCompositeFallbackFont FallbackTypeface;
	TArray<FCompositeSubFont> SubTypefaces;
};


struct UFont : UObject {
	// TODO
	
	/** What kind of font caching should we use? This controls which options we see */
	//EFontCacheType FontCacheType;
	uint8 FontCacheType;

	/** List of characters in the font.  For a MultiFont, this will include all characters in all sub-fonts!  Thus,
		the number of characters in this array isn't necessary the number of characters available in the font */
	TArray<struct FFontCharacter> Characters;

	/** Textures that store this font's glyph image data */
	//NOTE: Do not expose this to the editor as it has nasty crash potential
	TArray<class UTexture2D*> Textures;

	/** True if font is 'remapped'.  That is, the character array is not a direct mapping to unicode values.  Instead,
		all characters are indexed indirectly through the CharRemap array */
	int32 IsRemapped;

	/** Font metrics. */
	float EmScale;

	/** @todo document */
	float Ascent;

	/** @todo document */
	float Descent;

	/** @todo document */
	float Leading;

	/** Default horizontal spacing between characters when rendering text with this font */
	int32 Kerning;

	/** Options used when importing this font */
	struct FFontImportOptionsData ImportOptions;

	/** Number of characters in the font, not including multiple instances of the same character (for multi-fonts).
		This is cached at load-time or creation time, and is never serialized. */
	int32 NumCharacters;

	/** The maximum height of a character in this font.  For multi-fonts, this array will contain a maximum
		character height for each multi-font, otherwise the array will contain only a single element.  This is
		cached at load-time or creation time, and is never serialized. */
	TArray<int32> MaxCharHeight;

	/** Scale to apply to the font. */
	float ScalingFactor;

	/** The default size of the font used for legacy Canvas APIs that don't specify a font size */
	int32 LegacyFontSize;

	/** The default font name to use for legacy Canvas APIs that don't specify a font name */
	FName LegacyFontName;

	/** Embedded composite font data */
	FCompositeFont CompositeFont;

	/** This is the character that RemapChar will return if the specified character doesn't exist in the font */
	static const TCHAR NULLCHARACTER = 127;

	/** When IsRemapped is true, this array maps unicode values to entries in the Characters array */
//	TMap<uint16,uint16> CharRemap;
};