#include "PCI_APIC.h"
#include <iostream>

using namespace std;



PCI_APIC::PCI_APIC(): 

	PCIDevice(APIC_VENDOR_ID, APIC_DEVICE_ID,APIC_CLASS_NAME)

{

};