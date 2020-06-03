#ifndef CXX_CONFIG_H_
#define CXX_CONFIG_H_

#ifdef CXX_EXPORT
	#define CXX_API __declspec(dllexport)
#else
	#define CXX_API
#endif

#endif //CXX_CONFIG_H_