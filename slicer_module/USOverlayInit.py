# This module was tested on 3D Slicer version 4.3.1# To get the latest version of 3D Slicer, visit http://download.slicer.org# This file (or the corresponding .pyc file) should be located in# 'C:\Program Files\Slicer 4.3.1\lib\Slicer-4.3\qt-scripted-modules\PANavigationPythonLib.py'## IMPORTANT NOTICE: This version of PANavigationPythonLib.py is not compatible with 3D Slicer version 3.x### Sungmin Kim and Peter Kazanzides## This should be consistent with main CMakeLists.txt#SARRP_MODULE_VERSION = "3.1.8"import os, sys, math, json, time, socket, numpyimport unittestfrom __main__ import vtk, qt, ctk, slicerOIGTL_01_PORT = 18944OIGTL_01_CONNECT_NAME = 'DataTransfer_Probe_0'OIGTL_01_DEVICE_NAME = 'OpenIGTLink_USProbe'################################ GLOBAL FUNCTIONS ###################################################class USOverlayInit:    def __init__(self, parent):        self.module = self.__class__.__name__        parent.title = ""        parent.categories = []        parent.dependencies = []        parent.contributors = ["Andras Lasso (PerkLab, Queen's University), Steve Pieper (Isomics)"]                parent.helpText = """        """        parent.acknowledgementText = """        """            self.parent = parentclass USOverlayInitWidget:    def __init__(self, parent = None):        # Get module name by stripping 'Widget' from the class name        self.moduleName = self.__class__.__name__        if self.moduleName.endswith('Widget'):            self.moduleName = self.moduleName[:-6]            settings = qt.QSettings()            self.developerMode = settings.value('Developer/DeveloperMode').lower() == 'true'                if not parent:            self.parent = slicer.qMRMLWidget()            self.parent.setLayout(qt.QVBoxLayout())            self.parent.setMRMLScene(slicer.mrmlScene)        else:            self.parent = parent        self.layout = self.parent.layout()        if not parent:            self.setup()            self.parent.show()        self.scene = slicer.mrmlScene        self.scene.AddObserver(slicer.vtkMRMLScene.NodeAddedEvent, self.NodeAddedEvent)             #OpenIGTLink Server Setting        self.numIGTLConnectorNode = self.scene.GetNumberOfNodesByClass('vtkMRMLIGTLConnectorNode')                if self.numIGTLConnectorNode == 0:            self.oigtlConnector = slicer.vtkMRMLIGTLConnectorNode()            self.oigtlConnector.SetName(OIGTL_01_CONNECT_NAME)            #self.oigtlConnector.SetTypeServer(OIGTL_01_PORT)            self.oigtlConnector.SetTypeClient('localhost',OIGTL_01_PORT)                        self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.ConnectedEvent,    self.OpenIGTLinkConnected)            self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.DisconnectedEvent, self.OpenIGTLinkDisconnected)            self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.ActivatedEvent,    self.OpenIGTLinkActivated)            self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.DeactivatedEvent,  self.OpenIGTLinkDeactivated)            self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.ReceiveEvent,      self.OpenIGTLinkReceive)            self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.NewDeviceEvent,    self.OpenIGTLinkNewDevice)            slicer.mrmlScene.AddNode(self.oigtlConnector)            self.oigtlConnector.Start()                    else:            # self.numIGTLConnectorNode = self.scene.GetNumberOfNodesByClass('vtkMRMLIGTLConnectorNode')                    # when 'if self.numIGTLConnectorNode != 0 '                        node = self.scene.GetNodesByName(OIGTL_01_CONNECT_NAME)                        if node.GetNumberOfItems() == 0:                    self.oigtlConnector = slicer.vtkMRMLIGTLConnectorNode()                self.oigtlConnector.SetName(OIGTL_01_CONNECT_NAME)                #self.oigtlConnector.SetTypeServer(OIGTL_01_PORT)                self.oigtlConnector.SetTypeClient('localhost',OIGTL_01_PORT)                self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.ConnectedEvent,    self.OpenIGTLinkConnected)                self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.DisconnectedEvent, self.OpenIGTLinkDisconnected)                self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.ActivatedEvent,    self.OpenIGTLinkActivated)                self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.DeactivatedEvent,  self.OpenIGTLinkDeactivated)                self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.ReceiveEvent,      self.OpenIGTLinkReceive)                self.oigtlConnector.AddObserver(slicer.vtkMRMLIGTLConnectorNode.NewDeviceEvent,    self.OpenIGTLinkNewDevice)                slicer.mrmlScene.AddNode(self.oigtlConnector)                self.oigtlConnector.Start()    def onReload(self):        globals()[self.moduleName] = slicer.util.reloadScriptedModule(self.moduleName)    def setup(self):        self.reloadCollapsibleButton = ctk.ctkCollapsibleButton()        self.reloadCollapsibleButton.text = "Reload && Test"        self.layout.addWidget(self.reloadCollapsibleButton)        self.reloadFormLayout = qt.QFormLayout(self.reloadCollapsibleButton)               # reload button        # (use this during development, but remove it when delivering        #  your module to users)        self.reloadButton = qt.QPushButton("Reload")        self.reloadButton.toolTip = "Reload this module."        self.reloadButton.name = "ScriptedLoadableModuleTemplate Reload"        self.reloadFormLayout.addWidget(self.reloadButton)        self.reloadButton.connect('clicked()', self.onReload)       