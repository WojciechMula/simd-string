namespace bits {

    template <typename T>                                                                  
    unsigned get_first_bit_set(const T value) {                                            
                                                                                           
        assert(value != 0);                                                                
                                                                                           
        return __builtin_ctz(value);                                                       
    }          

} // namespace bits
