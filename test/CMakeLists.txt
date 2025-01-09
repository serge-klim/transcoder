find_package(Boost COMPONENTS unit_test_framework REQUIRED )

add_executable (transcoder_test
		transcoder_test.cpp
		dummy.hpp
)


target_link_libraries(transcoder_test
		fb::transcoder
	    Boost::disable_autolinking
        Boost::boost
		Boost::unit_test_framework
)

add_test(NAME test COMMAND transcoder_test)
