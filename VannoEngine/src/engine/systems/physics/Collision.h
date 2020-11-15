#pragma once
namespace VannoEngine {
	enum class Direction {
		TOP=0,
		RIGHT=1,
		BOTTOM=2,
		LEFT=3
	};

	enum class CollisionType {
		NONE=0,
		HARD=1,
		SOFT=2
	};

	class Collision {
	public:
		Collision();
		~Collision();

		void SetCollision(Direction dir, CollisionType type, float edge);
		CollisionType GetType(Direction dir) { return mType[int(dir)]; }
		float GetEdge(Direction dir) { return mEdge[int(dir)]; }
		bool HasCollided() { return mHasCollided; }
		bool CollisionDetected(Direction dir) { return mType[int(dir)] != CollisionType::NONE; }
	private:
		CollisionType mType[4];
		float mEdge[4];
		bool mHasCollided;
	};
}
