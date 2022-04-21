#ifndef __SIGABRT_NUMERIC_BENCHMARK__
#define __SIGABRT_NUMERIC_BENCHMARK__

#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#include <numeric/types/models.hpp>

#include <thesoup/types/types.hpp>

/**
 * \namespace numeric
 * 
 * \brief The root namespace.
 * */
namespace numeric {
    /**
     * \namespace numeric::benchmark
     * 
     * \brief Sub namespace with all numeric classes and functions.
     * */
    namespace benchmark {
        
        /**
         * \class Benchmark
         * 
         * \tparam Input The input type of your candidate function.
         * 
         * \tparam Output The output type of your candidate function.
         * 
         * \tparam It The iterator type for the inputs.
         * 
         * This class helps with benchmarking a callable (say DUT). This class needs the following to function:
         *   - A callable to benchmark.
         *   - An input generator function to generate inputs for the DUT.
         *   - Start and end points of forward iterator, which supplied the run parameters (more later).
         * 
         * The input sizes for a benchmark may vary from case to case. Also, for very small input sizes, the run time might
         * be so short that the measurement will be in accurate. hence you want to run many instances of the DUT in a loop
         * and take the average. On the flip side, you cannot use the same number of iterations for very large inputs without 
         * making the whole benchmarking process unnecessarily slow. Hence it's best if the benchmarking tool were supplied
         * with a (inputSize, numIterations) information to keep it as flexible as possible.
         * 
         * To compensate for the difficulty in setup of the benchmarking framework, there are Range classes that implement
         * input size - iteration mappings of logarithmic, linear and constant types.
         * 
         * The run times are saved in a map of input size vs run info.
         * 
         * */
        template <typename Input, typename Output, typename It> class Benchmark {
            static_assert(
                thesoup::types::IsForwardIteratorOfType<It, numeric::types::RunInfo>::value,
                "The inputs argument has to be a forward iterator of type RunInfo."
            );
        private:
            std::function<Input(const unsigned long&)> input_gen;
            std::function<Output(Input)> dut;
            It start;
            It end;
            std::map<unsigned long, numeric::types::RunInfo> run_times {};
        
        public:
            /**
             * \brief Cobstructor
             * 
             * Constructs a benchmark object.
             * 
             * \param input_gen (std::function) The input generator as explained before.
             * 
             * \param dut (std::function) The function under test.
             * 
             * \param start (forward_iterator) start iterator of the input ranges.
             * 
             * \param end (forward_iterator) end iterator of input ranges.
             * */
            Benchmark(
                const std::function<Input(const unsigned long&)>& input_gen,
                const std::function<Output(Input)>& dut,
                const It& start,
                const It& end
            ) : input_gen {input_gen}, dut {dut}, start {start}, end {end} {}

            
            /**
             * \brief Run the benchmark, and populate the results table.
             * 
             * This function runs the benchmarks and populates the runtimes in the input-size vs run info map.
             * 
             * \return A reference to self.
             * 
             * */
            const Benchmark<Input, Output, It>& run() {
                // Run through input sizes.
                for (auto it = start; it < end; it++) {
                    numeric::types::RunInfo run {*it};
                    
                    auto input1 {input_gen(run.inputSize)};
                    auto input2 {input_gen(run.inputSize)};
                    
                    // Run all iterations for that input size.
                    auto startTime {std::chrono::high_resolution_clock::now()};
                    for (unsigned long i = 0; i < run.iterations; i++) {
                        i%2 == 0? dut(input1) : dut(input2);
                    }
                    auto endTime {std::chrono::high_resolution_clock::now()};
                    
                    auto start {std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch()};
                    auto end {std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch()};
                    
                    run.runTimeInMillis = static_cast<double>((end - start).count())/static_cast<double>(run.iterations);
                    run_times[run.inputSize] = run;
                }
                return *this;
            }
            
            /**
             * \brief Get the results table.
             * 
             * This function returns a const reference to thr map of input size vs run infos.
             * 
             * \return (const& `std::map<unsigned long, RunInfo>`) The map with the results
             * */
            const std::map<unsigned long, numeric::types::RunInfo>& get_run_infos() const {
                return run_times;
            }
        };
    }
    
}

#endif
