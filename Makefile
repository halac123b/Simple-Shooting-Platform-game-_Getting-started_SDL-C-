SOURCE = main.cpp CommonFunc.cpp BaseObject.cpp TileMap.cpp Player.cpp Timer.cpp Bullet.cpp ThreatObject.cpp ExplosionObject.cpp TextObject.cpp PlayerStat.cpp Geometric.cpp BossObject.cpp
all:
	g++ -Isrc/include -Lsrc/lib -o main $(SOURCE) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

