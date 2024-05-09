// https://www.rfc-editor.org/rfc/rfc8032
// p = 2**255 - 19
// = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed
// = 57896044618658097711785492504343953926634992332820282019728792003956564819949
// order = 2**252 + 0x14def9dea2f79cd65812631a5cf5d3ed
// = 0x1000000000000000000000000000000014def9dea2f79cd65812631a5cf5d3ed
// = 7237005577332262213973186563042994240857116359379907606001950938285454250989
// cofactor = 8
//
// cv25519: v**2 = u**3 + a*u**2 + u mod p
// a = 486662
// = 0x76d06
// u = 9
// = 0x9
// v = 14781619447589544791020593568409986887264606134616475288964881837755586237401
// = 0x20ae19a1b8a086b4e01edd2c7748d14c923d4d7e6d7c61b229e9c5a27eced3d9
//
// ed25519: -x**2 + y**2 = 1 + d*x**2*y**2 mod p
// d = 37095705934669439343138083508754565189542113879843219016388785533085940283555
// = 0x52036cee2b6ffe738cc740797779e89800700a4d4141d8ab75eb4dca135978a3
// x = 15112221349535400772501151409588531511454012693041857206046113283949847762202
// = 0x216936d3cd6e53fec0a4e231fdd6dc5c692cc7609525a7b2c9562d608f25d51a
// y = 46316835694926478169428394003475163141307993866256225615783033603165251855960
// = 0x6666666666666666666666666666666666666666666666666666666666666658
//
// sqrt(156324) = 47404633599747791082622660160935871366498103638773495046873171379078394303736
// = 0x68ce126131538b6f95c59117d7277a48f9c5a6a70777247b129d5ae54d6454f8
//
// birational map
// u = (y - 1) / (y + 1)
// v = sqrt(156324) * u / x
//
// x = sqrt(156324) * u / v
// y = (1 + u) / (1 - u)

struct _ed25519_limb {
	x0: int;
	x1: int;
	x2: int;
	x3: int;
	x4: int;
	x5: int;
	x6: int;
	x7: int;
}

struct _ed25519_point {
	x: _ed25519_limb;
	y: _ed25519_limb;
}

ed25519_reduce(r: *int) {
	var c: int;
	var k: int;

	c = (r[0] + 19) >> 32;
	c = (c + r[1]) >> 32;
	c = (c + r[2]) >> 32;
	c = (c + r[3]) >> 32;
	c = (c + r[4]) >> 32;
	c = (c + r[5]) >> 32;
	c = (c + r[6]) >> 32;
	c = (c + r[7] + (1 << 31)) >> 32;

	k = -c >> 32;

	c = c + r[0] + (18 & k); r[0] = c & (-1 >> 32); c = c >> 32;
	c = c + r[1]; r[1] = c & (-1 >> 32); c = c >> 32;
	c = c + r[2]; r[2] = c & (-1 >> 32); c = c >> 32;
	c = c + r[3]; r[3] = c & (-1 >> 32); c = c >> 32;
	c = c + r[4]; r[4] = c & (-1 >> 32); c = c >> 32;
	c = c + r[5]; r[5] = c & (-1 >> 32); c = c >> 32;
	c = c + r[6]; r[6] = c & (-1 >> 32); c = c >> 32;
	c = c + r[7] + ((1 << 31) & k); r[7] = c & (-1 >> 32); c = c >> 32;
}

ed25519_add(r: *int, a: *int, b: *int) {
	var c: int;

	c = a[0] + b[0]; r[0] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] + b[1]; r[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] + b[2]; r[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] + b[3]; r[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] + b[4]; r[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[5] + b[5]; r[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[6] + b[6]; r[6] = c & (-1 >> 32); c = c >> 32;
	c = c + a[7] + b[7]; r[7] = c & (-1 >> 32); c = c >> 32;

	ed25519_reduce(r);
}

ed25519_sub(r: *int, a: *int, b: *int) {
	var c: int;
	c = 1 + a[0] + (-19 & (-1 >> 32)) + (~b[0] & (-1 >> 32)); r[0] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] + (-1 >> 32) + (~b[1] & (-1 >> 32)); r[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] + (-1 >> 32) + (~b[2] & (-1 >> 32)); r[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] + (-1 >> 32) + (~b[3] & (-1 >> 32)); r[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] + (-1 >> 32) + (~b[4] & (-1 >> 32)); r[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[5] + (-1 >> 32) + (~b[5] & (-1 >> 32)); r[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[6] + (-1 >> 32) + (~b[6] & (-1 >> 32)); r[6] = c & (-1 >> 32); c = c >> 32;
	c = c + a[7] + (-1 >> 33) + (~b[7] & (-1 >> 32)); r[7] = c & (-1 >> 33);
}

ed25519_mul(r: *int, a: *int, b: *int) {
	var _x: _ed25519_point;
	var x: *int;
	var c: int;
	var i: int;

	x = (&_x):*int;

	x[0] = 0;
	x[1] = 0;
	x[2] = 0;
	x[3] = 0;
	x[4] = 0;
	x[5] = 0;
	x[6] = 0;
	x[7] = 0;
	x[8] = 0;
	x[9] = 0;
	x[10] = 0;
	x[11] = 0;
	x[12] = 0;
	x[13] = 0;
	x[14] = 0;
	x[15] = 0;

	i = 0;
	loop {
		if i == 8 {
			break;
		}

		c = x[i + 0] + a[i] * b[0]; x[i + 0] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 1] + a[i] * b[1]; x[i + 1] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 2] + a[i] * b[2]; x[i + 2] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 3] + a[i] * b[3]; x[i + 3] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 4] + a[i] * b[4]; x[i + 4] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 5] + a[i] * b[5]; x[i + 5] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 6] + a[i] * b[6]; x[i + 6] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 7] + a[i] * b[7]; x[i + 7] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 8]; x[i + 8] = c & (-1 >> 32);

		i = i + 1;
	}

	c = x[0] + x[8] * 38; x[0] = c & (-1 >> 32); c = c >> 32;
	c = c + x[1] + x[9] * 38; x[1] = c & (-1 >> 32); c = c >> 32;
	c = c + x[2] + x[10] * 38; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + x[3] + x[11] * 38; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + x[4] + x[12] * 38; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + x[5] + x[13] * 38; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + x[6] + x[14] * 38; x[6] = c & (-1 >> 32); c = c >> 32;
	c = c + x[7] + x[15] * 38; x[7] = c & (-1 >> 32); c = c >> 32;

	ed25519_reduce(x);

	c = c * 38 + x[0]; x[0] = c & (-1 >> 32); c = c >> 32;
	c = c + x[1]; x[1] = c & (-1 >> 32); c = c >> 32;
	c = c + x[2]; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + x[3]; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + x[5]; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + x[6]; x[6] = c & (-1 >> 32); c = c >> 32;
	c = c + x[7]; x[7] = c & (-1 >> 32);

	ed25519_reduce(x);

	r[0] = x[0];
	r[1] = x[1];
	r[2] = x[2];
	r[3] = x[3];
	r[4] = x[4];
	r[5] = x[5];
	r[6] = x[6];
	r[7] = x[7];
}

ed25519_inv(r: *int, a: *int)  {
	var _x: _ed25519_point;
	var x: *int;
	var i: int;

	x = (&_x):*int;

	i = 0;
	loop {
		if i == 8 {
			break;
		}
		x[i] = a[i]; r[i] = a[i];
		i = i + 1;
	}

	i = 0;
	loop {
		if i == 254 {
			break;
		}
		ed25519_mul(r, r, r);
		if (i != 249 && i != 251) {
			ed25519_mul(r, r, x);
		}
		i = i + 1;
	}
}

ed25519_select(r: *int, a: *int, b: *int, k: int) {
	r[0] = (a[0] & ~k) | (b[0] & k);
	r[1] = (a[1] & ~k) | (b[1] & k);
	r[2] = (a[2] & ~k) | (b[2] & k);
	r[3] = (a[3] & ~k) | (b[3] & k);
	r[4] = (a[4] & ~k) | (b[4] & k);
	r[5] = (a[5] & ~k) | (b[5] & k);
	r[6] = (a[6] & ~k) | (b[6] & k);
	r[7] = (a[7] & ~k) | (b[7] & k);
	r[8] = (a[8] & ~k) | (b[8] & k);
	r[9] = (a[9] & ~k) | (b[9] & k);
	r[10] = (a[10] & ~k) | (b[10] & k);
	r[11] = (a[11] & ~k) | (b[11] & k);
	r[12] = (a[12] & ~k) | (b[12] & k);
	r[13] = (a[13] & ~k) | (b[13] & k);
	r[14] = (a[14] & ~k) | (b[14] & k);
	r[15] = (a[15] & ~k) | (b[15] & k);
}

ed25519_one(r: *int) {
	r[0] = 1;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;
	r[4] = 0;
	r[5] = 0;
	r[6] = 0;
	r[7] = 0;
}

////           x1 * y2 + x2 * y1                y1 * y2 - a * x1 * x2
//// x3 = ---------------------------,  y3 = ---------------------------
////       1 + d * x1 * x2 * y1 * y2          1 - d * x1 * x2 * y1 * y2
ed25519_pa(r: *int, a: *int, b: * int) {
	var _y1y2: _ed25519_limb;
	var y1y2: *int;
	var _x1x2: _ed25519_limb;
	var x1x2: *int;
	var _x1y2: _ed25519_limb;
	var x1y2: *int;
	var _x2y1: _ed25519_limb;
	var x2y1: *int;
	var _dxy: _ed25519_limb;
	var dxy: *int;
	var _dxy1: _ed25519_limb;
	var dxy1: *int;
	var _dxy2: _ed25519_limb;
	var dxy2: *int;
	var _d: _ed25519_limb;
	var d: *int;

	y1y2 = &_y1y2.x0;
	x1x2 = &_x1x2.x0;
	x1y2 = &_x1y2.x0;
	x2y1 = &_x2y1.x0;
	dxy = &_dxy.x0;
	dxy1 = &_dxy1.x0;
	dxy2 = &_dxy2.x0;
	d = &_d.x0;

	d[7] = (0x5203 << 16) | 0x6cee;
	d[6] = (0x2b6f << 16) | 0xfe73;
	d[5] = (0x8cc7 << 16) | 0x4079;
	d[4] = (0x7779 << 16) | 0xe898;
	d[3] = (0x0070 << 16) | 0x0a4d;
	d[2] = (0x4141 << 16) | 0xd8ab;
	d[1] = (0x75eb << 16) | 0x4dca;
	d[0] = (0x1359 << 16) | 0x78a3;

	ed25519_mul(y1y2, &a[8], &b[8]);
	ed25519_mul(x1x2, a, b);

	ed25519_mul(x1y2, a, &b[8]);
	ed25519_mul(x2y1, b, &a[8]);

	ed25519_mul(dxy, x1y2, x2y1);
	ed25519_mul(dxy, d, dxy);

	ed25519_one(dxy1);
	ed25519_add(dxy1, dxy1, dxy);
	ed25519_inv(dxy1, dxy1);

	ed25519_add(r, x1y2, x2y1);
	ed25519_mul(r, r, dxy1);

	ed25519_one(dxy2);
	ed25519_sub(dxy2, dxy2, dxy);
	ed25519_inv(dxy2, dxy2);

	ed25519_add(&r[8], y1y2, x1x2);
	ed25519_mul(&r[8], &r[8], dxy2);
}

ed25519_pk(r: *int, a: *int, k: *int) {
	var _b: _ed25519_point;
	var b: *int;
	var _c: _ed25519_point;
	var c: *int;
	var e: int;
	var i: int;
	var j: int;

	b = (&_b):*int;
	c = (&_c):*int;

	b[0] = a[0] & (-1 >> 32); r[0] = 0;
	b[1] = a[1] & (-1 >> 32); r[1] = 0;
	b[2] = a[2] & (-1 >> 32); r[2] = 0;
	b[3] = a[3] & (-1 >> 32); r[3] = 0;
	b[4] = a[4] & (-1 >> 32); r[4] = 0;
	b[5] = a[5] & (-1 >> 32); r[5] = 0;
	b[6] = a[6] & (-1 >> 32); r[6] = 0;
	b[7] = a[7] & (-1 >> 32); r[7] = 0;
	b[8] = a[8] & (-1 >> 32); r[8] = 1;
	b[9] = a[9] & (-1 >> 32); r[9] = 0;
	b[10] = a[10] & (-1 >> 32); r[10] = 0;
	b[11] = a[11] & (-1 >> 32); r[11] = 0;
	b[12] = a[12] & (-1 >> 32); r[12] = 0;
	b[13] = a[13] & (-1 >> 32); r[13] = 0;
	b[14] = a[14] & (-1 >> 32); r[14] = 0;
	b[15] = a[15] & (-1 >> 32); r[15] = 0;

	i = 7;
	loop {
		e = k[i];

		j = 0;
		loop {
			if j == 32 {
				break;
			}
			ed25519_pa(r, r, r);
			ed25519_pa(c, r, b);
			ed25519_select(r, r, c, -((e >> 31) & 1));
			e = e << 1;
			j = j + 1;
		}

		if i == 0 {
			break;
		}

		i = i - 1;
	}
}