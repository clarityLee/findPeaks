# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 0.0.3 - 2018-10-13
### Added
- Add more detailed execution time information to console for each steps in program.
### Changed
- Change the reading method to increase reading speed:
   from : ifstream file >> int , to custion read function.

## 0.0.2 - 2018-10-13
### Added
- Display execution time on console.

### Fixed
- Points at margins are now considered as possible peaks after the fix.
  The previous version did not consider points at margins as possible peaks.
- The directory of the output file "final.peak" is corrected to the same directory to the file "matrix.data".

### Changed
- Data type of several variables have been changed to short data type to reduce memory usage.

## 0.0.1 - 2018-10-12
### Added
- A basic structure for find peaks.