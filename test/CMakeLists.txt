find_package(Boost COMPONENTS unit_test_framework REQUIRED )

add_executable (transcoder_test
		traits_test.cpp
		transcoder_test.cpp
        sbe_test.cpp
        cme_eg.hpp
		dummy.hpp   
		sbe.hpp
)


target_link_libraries(transcoder_test
		transcoder::transcoder
	    Boost::disable_autolinking
        Boost::boost
		Boost::unit_test_framework
)

add_test(NAME test COMMAND transcoder_test)
