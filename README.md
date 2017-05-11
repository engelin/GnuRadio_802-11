# GnuRadio_802-11 
* 802.11a GnuRadio block
* Data-equalizing

## USRP B210 Setup
* Install dependency package
```bash
sudo apt-get update
sudo apt-get install emacs libboost1.48-all-dev libusb-1.0-0-dev python-cheetah doxygen python-docutils git cmake libncurses5-dev libfftw3-3 libfftw3-dev libfontconfig1-dev libxrender-dev libpulse-dev swig g++ automake autoconf libtool python-dev libfftw3-dev libcppunit-dev libusb-dev fort77 libsdl1.2-dev python-wxgtk2.8 git-core guile-1.8-dev libqt4-dev python-numpy ccache python-opengl libgsl0-dev python-cheetah python-lxml doxygen qt4-dev-tools libusb-1.0-0-dev libqwt5-qt4-dev libqwtplot3d-qt4-dev pyqt4-dev-tools python-qwt5-qt4 cmake git-core wget libxi-dev python-docutils gtk2-engines-pixbuf r-base-dev python-tk liborc-0.4-0 libasound2-dev python-gtk2 subversion liborc-dev libcomedi-dev python-sphinx
```

* Install UHD hardware driver
```bash
tar -zxvf release_003_009_001*
tar -zxvf uhd-3.9.1*
mv uhd-3.9.1 uhd
cd uhd
mkdir build cd build
cmake ../
make
make test
make install
```

* Path setting
```bash
echo "include /root/uhd/host/build/lib" >>/etc/ld.so.conf
ldconfig
```

*UHD image
```bash
tar -zxvf uhd-images_003.009.001-release
cd uhd-images*
cp -r share /home/rin/release_003_009_001/uhd/build
export UHD_IMAGES_DIR=/home/rin/release_003_009_001/uhd/build/share/uhd/images/
```

*Verify installation
```bash
./b2xx_fx3_utils
```
```bash
linux; GNU C++ version 4.6.3; Boost_104800; UHD_003.009.001-0-unknown

Device opened (VID=0x2500,PID=0x0020)
B2xx detected... Control of B2xx granted...

Loading firmware
-- Loading firmware image: /home/rin/release_003_009_001/uhd/build/share/uhd/images/usrp_b200_fw.hex... done
Device opened (VID=0x2500,PID=0x0020)
Firmware loaded
Operation complete!  I did it!  I did it!
```

```bash
# uhd_usrp_probe
```
```bash
\linux; GNU C++ version 4.6.3; Boost_104800; UHD_003.009.001-0-unknown

-- Detected Device: B210
-- Loading FPGA image: /home/rin/release_003_009_001/uhd/build/share/uhd/images/usrp_b210_fpga.bin... done
-- Operating over USB 2.
-- Detecting internal GPSDO.... No GPSDO found
-- Initialize CODEC control...
-- Initialize Radio control...
-- Performing register loopback test... pass
-- Performing register loopback test... pass
-- Performing CODEC loopback test... pass
-- Performing CODEC loopback test... pass
-- Asking for clock rate 16.000000 MHz... 
-- Actually got clock rate 16.000000 MHz.
-- Performing timer loopback test... pass
-- Performing timer loopback test... pass
-- Setting master clock rate selection to 'automatic'.
  _____________________________________________________
 /
|       Device: B-Series Device
|     _____________________________________________________
|    /
|   |       Mboard: B210
|   |   revision: 4
|   |   product: 2
|   |   serial: 30AC252
|   |   name: MyB210
|   |   FW Version: 8.0
|   |   FPGA Version: 11.0
|   |   
|   |   Time sources: none, internal, external, gpsdo
|   |   Clock sources: internal, external, gpsdo
|   |   Sensors: ref_locked
|   |     _____________________________________________________
|   |    /
|   |   |       RX DSP: 0
|   |   |   Freq range: -8.000 to 8.000 MHz
|   |     _____________________________________________________
|   |    /
|   |   |       RX DSP: 1
|   |   |   Freq range: -8.000 to 8.000 MHz
|   |     _____________________________________________________
|   |    /
|   |   |       RX Dboard: A
|   |   |     _____________________________________________________
|   |   |    /
|   |   |   |       RX Frontend: A
|   |   |   |   Name: FE-RX2
|   |   |   |   Antennas: TX/RX, RX2
|   |   |   |   Sensors: temp, rssi, lo_locked
|   |   |   |   Freq range: 50.000 to 6000.000 MHz
|   |   |   |   Gain range PGA: 0.0 to 76.0 step 1.0 dB
|   |   |   |   Bandwidth range: 200000.0 to 56000000.0 step 0.0 Hz
|   |   |   |   Connection Type: IQ
|   |   |   |   Uses LO offset: No
|   |   |     _____________________________________________________
|   |   |    /
|   |   |   |       RX Frontend: B
|   |   |   |   Name: FE-RX1
|   |   |   |   Antennas: TX/RX, RX2
|   |   |   |   Sensors: temp, rssi, lo_locked
|   |   |   |   Freq range: 50.000 to 6000.000 MHz
|   |   |   |   Gain range PGA: 0.0 to 76.0 step 1.0 dB
|   |   |   |   Bandwidth range: 200000.0 to 56000000.0 step 0.0 Hz
|   |   |   |   Connection Type: IQ
|   |   |   |   Uses LO offset: No
|   |   |     _____________________________________________________
|   |   |    /
|   |   |   |       RX Codec: A
|   |   |   |   Name: B210 RX dual ADC
|   |   |   |   Gain Elements: None
|   |     _____________________________________________________
|   |    /
|   |   |       TX DSP: 0
|   |   |   Freq range: -8.000 to 8.000 MHz
|   |     _____________________________________________________
|   |    /
|   |   |       TX DSP: 1
|   |   |   Freq range: -8.000 to 8.000 MHz
|   |     _____________________________________________________
|   |    /
|   |   |       TX Dboard: A
|   |   |     _____________________________________________________
|   |   |    /
|   |   |   |       TX Frontend: A
|   |   |   |   Name: FE-TX2
|   |   |   |   Antennas: TX/RX
|   |   |   |   Sensors: temp, lo_locked
|   |   |   |   Freq range: 50.000 to 6000.000 MHz
|   |   |   |   Gain range PGA: 0.0 to 89.8 step 0.2 dB
|   |   |   |   Bandwidth range: 200000.0 to 56000000.0 step 0.0 Hz
|   |   |   |   Connection Type: IQ
|   |   |   |   Uses LO offset: No
|   |   |     _____________________________________________________
|   |   |    /
|   |   |   |       TX Frontend: B
|   |   |   |   Name: FE-TX1
|   |   |   |   Antennas: TX/RX
|   |   |   |   Sensors: temp, lo_locked
|   |   |   |   Freq range: 50.000 to 6000.000 MHz
|   |   |   |   Gain range PGA: 0.0 to 89.8 step 0.2 dB
|   |   |   |   Bandwidth range: 200000.0 to 56000000.0 step 0.0 Hz
|   |   |   |   Connection Type: IQ
|   |   |   |   Uses LO offset: No
|   |   |     _____________________________________________________
|   |   |    /
|   |   |   |       TX Codec: A
|   |   |   |   Name: B210 TX dual DAC
|   |   |   |   Gain Elements: None
```

## Install Gnu Radio
* Gnu Radio v3.7.7
```bash
tar -zxvf gnuradio-3.6.4.1*
mv gnuradio-3.6.4.1 gnuradio
cd gnuradio
mkdir build cd build
cmake../
make
make test
make install
```

*Path setting
```bash
vi ~/.bashrc
export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python2.7/dist-packages
export PATH=/usr/local/bin:$PATH
ldconfig
```

*Run
```bash
gnuradio-companion
```

## Refer
* [IEEE 802.11p Transceiver](http://www.ccs-labs.org/bib/bloessl2013towards/bloessl2013towards.pdf)

* [WiSee](https://wisee.cs.washington.edu/wisee_paper.pdf)

