Wireless split keyboard firmware
======

(Note: this firmware works but is currently vulnerable to replay attacks.
It is provided as is and I don't guarantee it will receive further updates.)

Notes on Software Configuration
-------------------------------

Configuring the firmware is similar to any other TMK project. One thing
to note is that `MATIX_ROWS` in `config.h` is the total number of rows between
the two halves, i.e. if your split keyboard has 4 rows in each half, then
`MATRIX_ROWS=8`.

Also the current implementation assumes a maximum of 8 columns, but it would
not be very difficult to adapt it to support more if required.
