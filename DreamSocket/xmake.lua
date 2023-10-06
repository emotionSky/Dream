target("DreamSocket")    
    set_kind("$(kind)")
    set_basename("dream_socket")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
    add_files("src/**.cpp", "src/**.c")
    add_deps("DreamCommon")

    add_rules("utils.install.pkgconfig_importfiles")
    add_rules("utils.install.cmake_importfiles")