sub human_readable_size {
    my $number = shift;

    if ($number > 1099511627776) {  #   TiB: 1024 GiB
        return sprintf("%.2f TiB", $number / 1099511627776);
    }
    elsif ($number > 1073741824) {  #   GiB: 1024 MiB
        return sprintf("%.2f GiB", $number / 1073741824);
    }   
    elsif ($number > 1048576) {     #   MiB: 1024 KiB
        return sprintf("%.2f MiB", $number / 1048576);
    }   
    elsif ($number > 1024) {        #   KiB: 1024 B
        return sprintf("%.2f KiB", $number / 1024);
    }   
    else {                          #   bytes
        return "$number byte" . ($number == 1 ? "" : "s");
    }
}

