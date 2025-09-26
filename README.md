# tamagotchi
Raise a tiny virtual pet right from your terminal! Keep your buddy fed, happy, rested, clean, and healthy.

Features
Five core stats: Hunger, Happiness, Energy, Cleanliness, Health
Actions: Feed, Play, Sleep, Clean, Medicate, Wait
Events: Random sickness if neglected; healing & mood effects
Aging & mortality: Your choices matter—care well to thrive
ASCII mood faces: ^_^, -_-, T_T, (>_<), (x_x)
One file, zero deps: Easy to compile anywhere with a modern compiler

Quick Start
1) Get the code
tamagotchi.cpp
(Use the single file from this repo — or your customized version.)

3) Build
macOS (Apple Clang)
clang++ -std=c++17 -O2 tamagotchi.cpp -o tamagotchi
./tamagotchi

Linux (GCC/Clang)
g++ -std=c++17 -O2 tamagotchi.cpp -o tamagotchi
./tamagotchi

Windows (MSVC Developer Prompt)
cl /EHsc /std:c++17 tamagotchi.cpp
tamagotchi.exe

Windows (MinGW-w64)
g++ -std=c++17 -O2 tamagotchi.cpp -o tamagotchi.exe
tamagotchi.exe



Controls
Key	Action	Notes
1	Feed	Reduces hunger, small mess, small mood boost
2	Play	Big mood boost, costs energy, increases hunger
3	Sleep	Restores energy, small health boost, time passes more
4	Clean	Restores cleanliness; may cure sickness
5	Medicate	Cures sickness (if sick), small happiness penalty
6	Wait	Pass time gently
7	Stats	Show current status
8	Rename	Rename (and soft-restart if the pet has passed)
9	Help	Tips
0	Quit	Exit the game


How It Works 
Game advances in “ticks” whenever you act.
Low Energy/Cleanliness or high Hunger increases chance of sickness.
Health trends down slowly; faster if sick/starving/exhausted.
Age increases over time; the pet can die if badly neglected.
