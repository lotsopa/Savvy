{
  'targets': [
    {
      'target_name': 'mcpp',
      'product_prefix' : 'lib',
      'type': 'static_library',
      'sources':  [
        'directive.c',
        'eval.c',
        'expand.c',
        'main.c',
        'mbchar.c',
        'support.c',
        'system.c',
      ],
      'include_dirs' : ['.'],
      'defines' : [
        'HAVE_CONFIG_H',
        'MCPP_LIB=1'
      ],
      'configurations': {
        'Release': {
          'msvs_settings': {
            'VCCLCompilerTool': {
                'RuntimeLibrary': '2',
                'ExceptionHandling': '1',
                'RuntimeTypeInfo' : 'true'
              },
          },
          'msvs_disabled_warnings': [
            4018,
            4090,
            4101,
            4102,
            4133,
            4146,
            4244,
            4267
          ]
        }
      },
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            "MACOSX_DEPLOYMENT_TARGET":"10.9",
            'OTHER_CFLAGS': [
              '-fno-common',
              '-stdlib=libstdc++',
              '-w'
            ]
          }
        }],
        ['OS=="linux"', {
          'cflags' : [
            '-fPIC',
            '-w'
          ]
        }]
      ]
    }
  ]
}
