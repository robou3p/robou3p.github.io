package org.lakos.lakosbot;

public class Vector {
	float x;

	public float getX() {
		return x;
	}

	public void setX(float x) {
		this.x = x;
	}

	float y;

	public float getY() {
		return y;
	}

	public void setY(float y) {
		this.y = y;
	}

	float z;

	public float getZ() {
		return z;
	}

	public void setZ(float z) {
		this.z = z;
	}

	public Vector(float x, float y, float z) {
		super();
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public void setValue(float x, float y, float z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public void setValue(Vector anotherVector) {
		this.x = anotherVector.x;
		this.y = anotherVector.y;
		this.z = anotherVector.z;
	}

	public float getLength() {
		return (float) Math.sqrt(x * x + y * y + z * z);
	}

	public static float getLength(Vector vector) {
		return (float) Math.sqrt(vector.x * vector.x + vector.y * vector.y
				+ vector.z * vector.z);
	}

	public float dotProduct(Vector anotherVector) {
		return this.x * anotherVector.x + this.y * anotherVector.y + this.z
				* anotherVector.z;
	}

	public float getComponent(Vector anotherVector) {
		return dotProduct(anotherVector)
				/ (Vector.getLength(anotherVector) * Vector
						.getLength(anotherVector));
	}

	public Vector subtract(Vector anotherVector) {
		return new Vector(this.x - anotherVector.x, this.y - anotherVector.y,
				this.z - anotherVector.z);
	}
}
