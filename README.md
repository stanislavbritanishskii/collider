# Collider
Collision engine

this code requires glfw installed in ~/.brew/opt/

### Installation

git pull https://github.com/stanislavbritanishskii/collider/

### Usage
cd collider
make
./colider

### Contols
Q - will increase amount of energy lost per tick in absolute value
A - will decrease amount of energy lost per tick in absolute value
W - will increase amount of energy lost per tick in relative value
S - will decrease amount of energy lost per tick in relative value
T - will increase amount of substeps taken to solve collisions
G - will decrease amount of substeps taken to solve collisions
(if amount of substeps reaches zero, there will be no collision checks performed and particles will be able to pass through each other)
