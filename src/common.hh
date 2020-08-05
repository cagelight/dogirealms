#pragma once

#include <cstdint>

#include <array>
#include <atomic>
#include <memory>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <thread>

#include <meadow/strop.hh>
#include <meadow/time.hh>

#include <meadow/brassica.hh>
namespace b = meadow::brassica;

namespace DOGI {

	using timekeeper_t = meadow::time<CLOCK_MONOTONIC>::keeper;

	using Vec2 = b::vec2d;
	using Vec3 = b::vec3d;
	using Vec4 = b::vec4d;
	using Mat4 = b::mat4d;
	using Quat = b::quatd;

	using Vec2GL = b::vec2f;
	using Vec3GL = b::vec3f;
	using Vec4GL = b::vec4f;
	using Mat4GL = b::mat4f;

	using Vec2i = b::vec2<int_fast64_t>;
	using Vec3i = b::vec3<int_fast64_t>;
	using Vec4i = b::vec4<int_fast64_t>;
	
	extern std::atomic_bool run_sem;
	
	enum struct LogLevel {
		INFO,
		WARNING,
		ERROR
	};
	
	void print(std::string_view, LogLevel level = LogLevel::INFO);
}

#define LOGL(LEVEL) ( meadow::streamlogger { *[](std::string_view str){ DOGI::print(str, DOGI::LogLevel::LEVEL); } } << #LEVEL " [" << MEADOW_HERE << "] -- " )
#define LOGI LOGL(INFO)
#define LOGW LOGL(WARNING)
#define LOGE LOGL(ERROR)

#define LOGS ( meadow::streamlogger { *[](std::string_view str){ DOGI::print(str); } } )
