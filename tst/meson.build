matrixtest = executable('matrixtest', 'testmatrix.cc',
                    include_directories : inc)
                    
vectortest = executable('vectortest', 'testvector.cc',
                    include_directories : inc)

rreftest = executable('rreftest', 'testrref.cc',
                    include_directories : inc)
                    
benchmarktest = executable('benchmarktest', 'testbenchmark.cc',
                    include_directories : inc)
                    
fractiontest = executable('fractiontest', 'testfractions.cc',
                    include_directories : inc)

test('Matrix test', matrixtest)
test('Vector test', vectortest)
test('RREF test', rreftest)
test('Benchmark test', benchmarktest)
test('Fraction test', fractiontest)

