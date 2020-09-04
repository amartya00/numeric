matrixtest = executable('matrixtest', 'testmatrix.cc',
                    include_directories : inc)
                    
vectortest = executable('vectortest', 'testvector.cc',
                    include_directories : inc)

rreftest = executable('rreftest', 'testrref.cc',
                    include_directories : inc)

test('Matrix test', matrixtest)
test('Vector test', vectortest)
test('RREF test', rreftest)

