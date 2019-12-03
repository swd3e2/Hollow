#include <memory>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#ifdef _DEBUG

#define DEBUG_BREAK() asm { int 3 }

#define ASSERT(expr) \
	if (expr) {} \
	else \
	{ \
		reportAssertionFailure(#expr, \
				__FILE, __LINE__); \
		DEBUG_BREAK(); \
	}

#define _ASSERT_GLUE(a, b) a ## b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)

#define STATIC_ASSERT(expr) \
	enum \
	{ \
		ASSERT_GLUE(g_assert_fail, __LINE__) \
			= 1 / (int)(!!(expr)) \
	}
#else
#define ASSERT(expr)
#define STATIC_ASSERT(expr)
#endif


namespace Hollow {
	using UINT32 = unsigned int;

	template <typename T>
	using s_ptr = std::shared_ptr<T>;

	template <typename T>
	using w_ptr = std::weak_ptr<T>;

	template <typename T>
	using u_ptr = std::unique_ptr<T>;
}
