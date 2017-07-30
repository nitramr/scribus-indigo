

/*
 * Prevent code from including config.h directly. You should include
 * scconfig.h instead. This lets us handle win-config.h correctly, and
 * may do more later.
 */
#ifndef _SCCONFIG_H
#error "Include <scconfig.h> rather than using <config.h> directly"
#endif

#cmakedefine VERSION "1.5.4.svn"

#cmakedefine BUILD_WINDOWS_VERSION 1
#cmakedefine DLL_USE_NATIVE_API 1
#cmakedefine BUILD_MAC_BUNDLE
#cmakedefine HAVE_CMS 1
#cmakedefine HAVE_LCMS2 1
#cmakedefine CMS_INC @CMS_INC@
#cmakedefine HAVE_XML 1
#cmakedefine HAVE_CAIRO 1
#cmakedefine HAVE_CUPS 1
#cmakedefine HAVE_TIFF 1
#cmakedefine HAVE_LIBZ 1
#cmakedefine HAVE_FONTCONFIG 1
#cmakedefine HAVE_PODOFO 1
#cmakedefine HAVE_POPPLER 1
#cmakedefine HAVE_OSG 1
#cmakedefine HAVE_MEMRCHR 1
#cmakedefine HAVE_BOOST 1
#cmakedefine HAVE_SINCOS
#cmakedefine HAVE_ROUND
#cmakedefine HAVE_TRUNC
#cmakedefine HAVE_SYS_TYPES_H 1
#cmakedefine HAVE_SYS_STAT_H 1
#cmakedefine FT_FREETYPE_H
#cmakedefine COMPILE_PYTHON 1
#cmakedefine WORDS_BIGENDIAN 1
#cmakedefine GMAGICK_FOUND 1
#cmakedefine WITH_TESTS 1
#cmakedefine HAVE_REVENGE 1
#cmakedefine HUNSPELL_NEWAPI "@HUNSPELL_NEWAPI@"
