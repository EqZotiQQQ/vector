Vector realization. Looks like its little bit faster then std::vector

--------------------------------------------------------
Benchmark                           Time             CPU
--------------------------------------------------------
vector_push_back_no_std        244289 us       244243 us
vector_push_back_std           248754 us       248717 us
vector_emplace_back_no_std     100085 us       100066 us
vector_emplace_back_std        115232 us       115213 us
