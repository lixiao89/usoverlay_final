# usoverlay_final
a project that uses slicer and polaris for ultrasound probe tracking. integrated slicer python module and cisst (mtsNDITracker, mtsOpenIGTLink, etc).



Usage:
To use "clarity_sim" with "usoverlay"

find the executable 'NDITracker', run './NDITracker -op -cp '

find executable 'ClarityPolarisClient', run './ClarityPolarisClient -cp ' (on ubuntu this command needs to be run under sudo), note that the two clarity port numbers should be the same. remember to edit config.xml to the correct serial port.

note1: server bears a port number, client bears the ip address of the server and the same port number
note2: TCP port number sometimes needs to be changed after a few runs, need to correctly close the ports after each use

""""
