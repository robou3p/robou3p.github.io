package org.lakos.lakosbot;

public class Vector {

	public float x;
	public float y;
	public float z;

	public Vector() {
	    x = 0;
	    y = 0;
	    z = 0;
    }

	public Vector(float x, float y, float z) {
		super();
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Vector(Vector anotherVector) {
		x = anotherVector.x;
		y = anotherVector.y;
		z = anotherVector.z;
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
