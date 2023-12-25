# GIS-based Emergency Vehicle Routing System
## Description
This project develops a GIS-based application for emergency call centers to quickly determine the
shortest path for ambulances to reach users in need. It uses Dijkstra's algorithm to calculate the
shortest route from the user's location to the nearest hospital. The system is designed to aid rapid
decision-making in critical situations and operates independently of an active internet connection.
## Features
- **Nearest Hospital Location**: Identifies the closest hospital to the user's location.
- **Path Calculation**: Computes the shortest path using Dijkstra's algorithm.
- **Emergency Response Optimization**: Facilitates faster ambulance dispatch and routing.
## Installation
To run this project, you need Python and a C compiler like GCC. Follow these steps to set it up:
```
git clone https://github.com/kickereb/Ambulance-Service/edit/master/README.md
gcc -o routing_system final_project.c -lm
python -m pip install gmplot
```
## Usage
Run the compiled C program to calculate paths and use Python's gmplot package for visualizing
routes on a map.
## Contributing
Contributions to this project are welcome. Please fork the repository, make your changes, and
submit a pull request for review.
## License
This project is licensed under the MIT License. See the LICENSE file in the repository for more
details.
## Limitations
- The path calculation does not currently account for real-time traffic or road conditions.
- The application is tailored for Shivajinagar, Pune, but can be adapted for other locations.
