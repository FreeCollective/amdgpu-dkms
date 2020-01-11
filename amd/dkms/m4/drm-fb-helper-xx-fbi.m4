dnl #
dnl # commit b8017d6c33be9862505a9154e302c4b00cbfca43
dnl # drm/fb_helper: Add drm_fb_helper functions to manage fb_info creation
dnl #
AC_DEFUN([AC_AMDGPU_DRM_FB_HELPER_XX_FBI],
	[AC_MSG_CHECKING([whether drm_fb_helper_{alloc/unregister}_fbi() is available])
	AC_KERNEL_TRY_COMPILE_SYMBOL([
		#include <drm/drm_fb_helper.h>
	],[
		drm_fb_helper_alloc_fbi(NULL);
		drm_fb_helper_unregister_fbi(NULL);
	],[drm_fb_helper_alloc_fbi drm_fb_helper_unregister_fbi],[drivers/gpu/drm/drm_fb_helper.c],[
		AC_MSG_RESULT(yes)
		AC_DEFINE(HAVE_DRM_FB_HELPER_XX_FBI, 1, [drm_fb_helper_{alloc/unregister}_fbi is available])
	],[
		AC_MSG_RESULT(no)
	])
])