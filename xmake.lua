set_config("plat", os.host())
set_project("DreamSky")
set_version("1.0.0", {build = "%Y%m%d%H%M"})
set_xmakever("2.3.1")
set_languages("cxx11", "c11")
set_warnings("all")

add_defines("DREAMSKY_EXPORTS")

function dream_directory() 
    local res = {}
    for _, dream in ipairs(os.dirs("./*")) do
        local tmp_dir = path.join(dream, "include")
        if (os.exists(tmp_dir)) then
            table.insert(res, dream)
        end
    end
    return res
end

for _, dream in ipairs(dream_directory()) do 
    dream = dream .. "/include"
    add_includedirs(dream)
end

add_requires("rapidjson")
if is_os("windows") then 
    add_requires("pthreads4w")
end

for _, dream in ipairs(dream_directory()) do 
    includes(dream)
end
