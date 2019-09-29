#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CARIMGPROCESS_LIB)
#  define CARIMGPROCESS_EXPORT Q_DECL_EXPORT
# else
#  define CARIMGPROCESS_EXPORT Q_DECL_IMPORT
# endif
#else
# define CARIMGPROCESS_EXPORT
#endif
