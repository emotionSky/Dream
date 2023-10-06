target("DreamLog")    
    set_kind("$(kind)")
    set_basename("dream_log")
    add_includedirs("include", {public = true})
    add_includedirs("src")
    add_headerfiles("include/(**.h)")
    add_files("src/**.cpp")

    add_rules("utils.install.pkgconfig_importfiles")
    add_rules("utils.install.cmake_importfiles")