target("DreamThread")    
    set_kind("$(kind)")
    set_basename("dream_thread")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
    add_files("src/**.cpp")
    add_deps("DreamCommon")

    if is_os("windows") then 
        add_packages("pthreads4w")
    end

    add_rules("utils.install.pkgconfig_importfiles")
    add_rules("utils.install.cmake_importfiles")