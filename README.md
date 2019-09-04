# AI bot for the game of Yinsh

This repo contains the code for an AI bot, for playing a game of Yinsh. Yinsh is an abstract strategy board two-player game.
**Algorithm**: It uses adversarial search algorithm Mini-max with Alpha-Beta pruning.

## Rules
The rules of the game can be found [here](http://www.gipf.com/yinsh/rules/rules.html)

## Run Instructions
Here are the sample instructions used to match two random players against each other over the server network.
### Setup Server
`python server.py 10000 -n 5 -NC 2 -TL 150 -LOG server.log`
### Setup Client 1
`python  client.py 0.0.0.0 10000 RandomPlayer.py -mode GUI`
### Setup Client 2
`python  client.py 0.0.0.0 10000 RandomPlayer.py`