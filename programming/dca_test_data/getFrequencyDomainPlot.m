function [x, y] = getFrequencyDomainPlot( adc_data, sampling_freq, adc_bits )
    T = 1 / sampling_freq;
    L = length( adc_data );
    t = ( 0 : L - 1 ) * T;
    
    Y = fft( adc_data );
    
    P2 = abs( Y / L );
    P1 = P2( 1 : L / 2 + 1 );
    P1( 2 : end - 1 ) = 2 * P1( 2 : end - 1 );
    P1 = 20 * log10( abs( P1 ) / ( 2 ^ adc_bits ) );    % Calculate dBfs.
    
    f = sampling_freq * ( 0 : ( L / 2 ) ) / L;
    
    % Return the data
    x = f;
    y = P1;
end
