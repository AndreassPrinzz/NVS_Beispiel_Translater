project('Translator', 'cpp',
        license : ['Boost'],
        version : '0.1',
        default_options : ['cpp_std=c++17', '-wError=pendantic'])

add_global_arguments('-I' + get_option('asio_include_dir'), language : 'cpp')
add_global_arguments('-DASIO_STANDALONE', language : 'cpp')
add_global_arguments('-DASIO_NO_DEPRECATED', language : 'cpp')

add_global_arguments('-I' + get_option('cli11_include_dir'), language : 'cpp')

add_global_arguments('-I' + get_option('spdlog_include_dir'), language : 'cpp')

inc_dir = include_directories(['include'])


thread_dep = dependency('threads')

src_server = ['src/server/main.cpp']

src_client = ['src/client/main.cpp']

executable('server',
            sources : src_server,
            include_directories : inc_dir,
            dependencies : thread_dep,
            #link_with : XXX_lib
           )

executable('client',
           sources : src_client,
           include_directories : inc_dir,
           dependencies : thread_dep
           )