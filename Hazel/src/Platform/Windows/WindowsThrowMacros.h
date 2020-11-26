#pragma once

// Error exception helper macro
#define HZWND_EXCEPT(hr) WindowsWindow::HrException(__LINE__, __FILE__, hr)
#define HZWND_LAST_EXCEPT() WindowsWindow::HrException(__LINE__, __FILE__, GetLastError())
#define HZWND_NOGFX_EXCEPT() WindowsWindow::NoGraphicsException(__LINE__, __FILE__)