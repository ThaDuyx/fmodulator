# fmodulator
> Mini project for the '[Sound Processing](https://moduler.aau.dk/course/2022-2023/MSNSMCM1202?lang=en-GB)' course @AAU, Cph 2023/2024

## Description
This paper discusses the implementation of John M. Chowning’s traditional FM synthesis algorithm within the JUCE platform as a MIDI plugin. It includes both a detailed description of the algorithm and a technical analysis. While a complete implementation of J. Chowning's algorithm was not fully realized, the project successfully established the necessary framework for a MIDI synthesizer performing basic frequency modulation.

## Algorithm
The simplest form that describe the relationship of the carrier and modulator signal in FM synthesis is the following equation:
$$y(t)=A_{carrier} \cdot sin(2 \cdot \pi \cdot f_{carrier} \cdot t + A_{mod} \cdot (2 \cdot \pi f_{mod} \cdot t))$$
