#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CARINFORMATION_LIB)
#  define CARINFORMATION_EXPORT Q_DECL_EXPORT
# else
#  define CARINFORMATION_EXPORT Q_DECL_IMPORT
# endif
#else
# define CARINFORMATION_EXPORT
#endif
