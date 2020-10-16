function [retVal] = readDCA1000( fileName )
    numADCSamples = 256;    % Number of ADC samples per chirp.
    numADCBits = 16;        % Number of ADC bits per sample.
    numRX = 4;              % Using 4 receivers.
    numLanes = 2;           % Always using 2 LVDS lanes.
    isReal = 1;             % Set to 1 if real only data, 0 if complex data
    
    
    % Read the .bin file.
    fid = fopen( fileName, 'r' );       % Open the file.
    adcData = fread( fid, 'int16' );    % Read the contents as 16-bit chunks of data.
    
    if numADCBits ~= 16
            l_max = 2^(numADCBits - 1) - 1;
            adcData( adcData > l_max ) = adcData( adcData > l_max ) - 2^numADCBits;
    end
    
    fclose( fid );
    fileSize = size( adcData, 1 );
    
    % Real data reshape, file size = numADCSamples * numChirps
    if isReal
        numChirps = fileSize / numADCSamples / numRX;
        LVDS = zeros( 1, fileSize );
        
        % Create a column for each chirp
        LVDS = reshape( adcData, numADCSamples * numRX, numChirps );
        
        % Each row is data from one chirp.
        LVDS = LVDS.';
    else
        % For complex data.
        % file size = 2  * numADCSamples * numChirps
        numChirps = fileSize / 2 / numADCSamples / numRX;
        LVDS = zeros( 1, fileSize / 2 );
        
        % Combine real and imaginary parts into complex data.
        % Read in file: 2I is followed by 2Q.
        counter = 1;
        for i = 1:4:fileSize - 1
            LVDS( 1, counter )      = adcData( i )      + sqrt( -1 ) * adcData( i + 2 );
            LVDS( 1, counter + 1 )  = adcData( i + 1 )  + sqrt( -1 ) * adcData( i + 3 );
            counter = counter + 2;
        end
        
        % Create a column for each chirp
        LVDS = reshape( LVDS, numADCSamples * numRX, numChirps );
        
        % Each row is data from one chirp.
        LVDS = LVDS.';
    end
    
    % Organize data per RX.
    adcData = zeros( numRX, numChirps * numADCSamples );
    for row = 1:numRX
        for i = 1:numChirps
            adcData( row, ( i - 1 ) * numADCSamples + 1 : i * numADCSamples ) = LVDS( i, ( row - 1 ) * numADCSamples + 1 : row * numADCSamples );
        end
    end
    
    % Return the receiver data.
    retVal = adcData;
end
    