# 🚗 OMNeT++ Workbench with SUMO & Veins Integration

This workbench integrates OMNeT++, SUMO, and Veins to simulate vehicular networks. It enables co-simulation of network and traffic dynamics using the TraCI interface.

## 📦 Prerequisites

Ensure the following components are installed:

- **OMNeT++**: Version 5.6 or later.
- **SUMO**: Version 1.8 or later.
- **Veins**: Version 5.1 or later.
- **Python**: Version 3.7 or later. ([Modules - Veins simulation](https://veins.car2x.org/documentation/modules/?utm_source=chatgpt.com))

For detailed installation instructions, refer to the [Veins Instant Setup Guide](https://veins.car2x.org/documentation/instant-veins/).

## 📁 Installation Steps

1. **Clone the Workbench Repository**

   ```bash
   git clone https://github.com/yourusername/your-workbench.git
   ```


2. **Import Projects into OMNeT++**

   - Open the OMNeT++ IDE.
   - Go to `File` > `Import` > `General` > `Existing Projects into Workspace`.
   - Select the cloned repository folder.
   - Ensure all projects are checked and click `Finish`.

3. **Import Veins and Subprojects**

   - Download Veins (e.g., version 5.3.1) and unpack it.
   - In the OMNeT++ IDE, go to `File` > `Import` > `General` > `Existing Projects into Workspace`.
   - Select the Veins directory and check the "Search for nested projects" box.
   - This will add both `veins` and `veins_inet` projects. Click `Finish`. ([Tutorial - Veins simulation](https://veins.car2x.org/tutorial/?utm_source=chatgpt.com), [org.car2x.veins.subprojects.veins_inet.VeinsInetManager](https://groups.google.com/g/omnetpp/c/WVHoQ6J-004?utm_source=chatgpt.com))

4. **Build the Projects**

   - Right-click on each imported project and select `Build Project`.

## 🚀 Running the Simulation

1. **Start the SUMO Server**

   Veins uses a Python script to launch the SUMO server. Run the following command in your terminal:

   ```bash
   python /path/to/veins/bin/sumo-launchd.py -vv -c /path/to/sumo/bin/sumo
   ```


   Replace `/path/to/veins` and `/path/to/sumo` with the actual installation paths.

   This command starts the SUMO server with verbose output, allowing OMNeT++ to connect via the TraCI interface.

2. **Run the OMNeT++ Simulation**

   - In the OMNeT++ IDE, navigate to the `omnetpp.ini` file of your project.
   - Right-click on `omnetpp.ini` and select `Run As` > `OMNeT++ Simulation`.

   The simulation will start, and OMNeT++ will communicate with SUMO to simulate vehicular movements and network interactions.

## 🧪 Simulation Types

This workbench includes multiple simulation configurations:

- **`NJS_ap`**: Standard Brownian motion simulation without SUMO.
- **`veins_inet`**: Erlangen, Germany simulation (requires SUMO).
- **`veins_inet_durham_demo`**: Durham, UK simulation (requires SUMO).

To run a specific simulation:

1. Open the corresponding `omnetpp.ini` file.
2. Ensure the SUMO server is running if required.
3. Right-click on `omnetpp.ini` and select `Run As` > `OMNeT++ Simulation`.
## 🛠 Troubleshooting

- **SUMO Server Connection Issues**: If OMNeT++ cannot connect to the SUMO server, verify that `sumo-launchd.py` is running and listening on the correct port (default is 9999).
- **TraCI Errors**: Ensure that the versions of OMNeT++, SUMO, and Veins are compatible. Incompatible versions can lead to TraCI communication errors.

## 📚 Additional Resources

- [Veins Documentation](https://veins.car2x.org/documentation/)
- [SUMO Documentation](https://sumo.dlr.de/docs/)
- [OMNeT++ Documentation](https://doc.omnetpp.org/)