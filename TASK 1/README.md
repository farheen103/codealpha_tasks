A simple and interactive Smart Calculator developed using the C programming language and Windows API (Win32). It provides a graphical user interface (GUI) for performing basic arithmetic operations.

✨**Features**
➕ Addition — Adds two numbers.
➖ Subtraction — Subtracts one number from another.
✖️ Multiplication — Multiplies two numbers.
➗ Division — Divides two numbers with division-by-zero validation.
🧹 Clear — Clears the input fields and resets the result.
❌ Exit — Closes the application.
🎨 Colorful GUI — User-friendly interface with custom colors and fonts.
⚠️ Input Validation — Displays error messages for invalid inputs.

📋**Requirements**
Windows operating system
GCC compiler (MinGW)
Visual Studio Code or another C code editor

🚀**How to Compile and Run**

1. Clone or Download the Repository
Download the project files from this GitHub repository.

2. Open the Project Folder
Open the folder containing calculator.c in Visual Studio Code.

3. Compile the Program
Open the VS Code terminal and run:
gcc calculator.c -o SmartCalculator.exe -mwindows -lgdi32 -luser32 -lm

4. Run the Application
.\SmartCalculator.exe

The calculator will open in a separate Windows application window.
