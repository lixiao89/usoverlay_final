from __main__ import vtk, qt, ctk, slicer

#
# PANavigationPython
#
import PANavigationPythonGUILib

class PANavigationPython:
    def __init__(self, parent):
        parent.title = "PANavigation Python"
        parent.categories = ["IGT"]
        parent.dependencies = []
        parent.contributors = ["Sungmin Kim (Johns Hopkins University)",
                               "Peter Kazanzides (Johns Hopkins University)"]
        parent.helpText = """
            The PANavigationPython module implemented to assis Photo-acoustic imaging with Python
            """
        parent.acknowledgementText = """
            This file was originally developed by Sungmin Kim and Peter Kazanzides. This work was supported by ... grant(grant #)."""
        self.parent = parent

#
# qPANavigationPythonWidget
#

class PANavigationPythonWidget(PANavigationPythonGUILib.PANavigationPythonGUIInternal):
    def __init__(self, parent = None):
        PANavigationPythonGUILib.PANavigationPythonGUIInternal.__init__(self, parent)
