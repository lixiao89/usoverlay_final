from __main__ import vtk, qt, ctk, slicer

#
# USOverlay
#
import USOverlayGUILib

class USOverlay:
    def __init__(self, parent):
        parent.title = "USOverlay"
        parent.categories = ["IGT"]
        parent.dependencies = []
        parent.contributors = ["Xiao Li (Johns Hopkins University)",
                               "Sungmin Kim (Johns Hopkins University)",
                               "Peter Kazanzides (Johns Hopkins University)"]
        parent.helpText = """
            The USOverlay module implemented to diplay ultrasound transducer and ultrasound image plane on the 3D view with tracking information
            """
        parent.acknowledgementText = """
            This file was originally developed by Xiao Li, Sungmin Kim and Peter Kazanzides. This work was supported by ... grant(grant #)."""
        self.parent = parent

#
# qUSOverlayWidget
#

class USOverlayWidget(USOverlayGUILib.USOverlayGUIInternal):
    def __init__(self, parent = None):
        USOverlayGUILib.USOverlayGUIInternal.__init__(self, parent)
