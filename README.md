## Motivation

Wireless data transmission over RF has become ubiqutous. Regadless of a particular application, the data has to be decoded error-free after being transmitted over a wireless channel, and this is crutial. 

In practice any transmission channel is affected by noise.
Considering the noise is characterized as Gaussian, there is a limit that allows the maximum error free transmission rate, possible for a given signal-to-noise ratio (SNR) and channel bandwidth, which is called as Shannon’s Limit.

The noise within the channel can corrupt data during transmission. To eliminate this issue, error correcting codes are leveraged to reveal and correct the errors at receiver side after the data is transmitted. All kinds of error correcting codes are based on incorporation of some redundancy bits to transmitted data, though very different algorithms are used for data encoding and decoding. Nevertheless, all of error correcting codes aspire to get closer to the Shannon Limit for data transmission.

## Synopsis

This library implements a bundle of various data coding approaches well suitable for wireless data communication over RF.

It is designated to be deployed on embedded devices with relatively modest computing capabilities, in connection with that it leverages fast algorithms and requires a quite small memory footprint.

By the moment the library exposes the following functions:
* Data encoding/decoding with Hamming codes
* Data encoding/decoding with Reed-Solomon codes (only encoder part has been implemented so far, work in progress)
* CCITT data whitening

### Hamming codes

Hamming codes are in a class of binary linear error-correcting codes, that capable to correct one-bit errors.

### Reed-Solomon codes

TBD

### Data whitening

In wireless RF systems a data to be transmitted may contain long sequences of 0 and 1, that will introduce occurences of a DC bias into transmitted signal. Such thr DC-biased signal will have a non-uniform power distribution over the transmittion channel bandwidth. The DC bias will also introduce data dependencies during an operation of the demodulator. It is higly desirable and optimal for the transmitted data to be random and DC free.

The data whitening is applied to randomize transmitted data bits and provide a near-uniform power distribution.

## How to run tests

Navigate to the project's root directory and:

```
mkdir build
cd build
cmake -DTARGET_GROUP=test ..
make
make test
```
