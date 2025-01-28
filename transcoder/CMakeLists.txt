find_package(Boost REQUIRED )

add_library (transcoder INTERFACE)

add_library (fb::transcoder ALIAS transcoder)

target_sources(transcoder PUBLIC FILE_SET HEADERS
				FILES
				detail/sint.hpp
				detail/describe.hpp
				detail/tuple.hpp
				detail/utils.hpp
				detail/type_order.hpp
				detail/byte_order.hpp
				detail/sizeof.hpp
				detail/encoder.hpp
				detail/decoder.hpp
				detail/vdecoder.hpp
				detail/options.hpp
                detail/utils/sizeof.hpp
				detail/utils/utils.hpp
#				detail/type_selector.hpp
				error.hpp
				traits.hpp
				basic_types.hpp
				options.hpp
				byte_order.hpp
				type_name.hpp
                transcoder.hpp
				flyweight.hpp
				io.hpp
			   )

target_include_directories(transcoder INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:include>
		)

target_link_libraries(transcoder INTERFACE Boost::boost)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
							${CMAKE_CURRENT_BINARY_DIR}/transcoder-config.cmake
							INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/transcoder
							NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/transcoder-config-version.cmake
		VERSION ${CMAKE_PROJECT_VERSION}
		COMPATIBILITY AnyNewerVersion
)

install(TARGETS transcoder
        EXPORT transcoder-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/transcoder
)

install(FILES 
			${CMAKE_CURRENT_BINARY_DIR}/transcoder-config.cmake
			${CMAKE_CURRENT_BINARY_DIR}/transcoder-config-version.cmake
		DESTINATION 
		    ${CMAKE_INSTALL_DATADIR}/transcoder
)

install(EXPORT transcoder-targets
		FILE transcoder-targets.cmake
		NAMESPACE fb::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/transcoder
)
