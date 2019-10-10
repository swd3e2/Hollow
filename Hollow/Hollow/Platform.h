#include  <memory>

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

namespace Hollow {
	using UINT = unsigned int;

	template <typename T>
	using s_ptr = std::shared_ptr<T>;

	template <typename T>
	using w_ptr = std::weak_ptr<T>;

	template <typename T>
	using u_ptr = std::unique_ptr<T>;
}
