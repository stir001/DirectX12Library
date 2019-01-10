matrix inverse(matrix mat)
{
    matrix ret;

    float det = determinant(mat);

    float3x3 m33;
	//11
    m33._11_12_13 = mat._22_23_24;
    m33._21_22_23 = mat._32_33_34;
    m33._31_32_33 = mat._42_43_44;
    ret._11 = determinant(m33);

	//12
    m33._11_12_13 = mat._21_23_24;
    m33._21_22_23 = mat._31_33_34;
    m33._31_32_33 = mat._41_43_44;
    ret._21 = -determinant(m33);

	//13
    m33._11_12_13 = mat._21_22_24;
    m33._21_22_23 = mat._31_32_34;
    m33._31_32_33 = mat._41_42_44;
    ret._31 = determinant(m33);

	//14
    m33._11_12_13 = mat._21_22_23;
    m33._21_22_23 = mat._31_32_33;
    m33._31_32_33 = mat._41_42_43;
    ret._41 = -determinant(m33);

	//21
    m33._11_12_13 = mat._12_13_14;
    m33._21_22_23 = mat._32_33_34;
    m33._31_32_33 = mat._42_43_44;
    ret._12 = -determinant(m33);

	//22
    m33._11_12_13 = mat._11_13_14;
    m33._21_22_23 = mat._31_33_34;
    m33._31_32_33 = mat._41_43_44;
    ret._22 = determinant(m33);

	//23
    m33._11_12_13 = mat._11_12_14;
    m33._21_22_23 = mat._31_32_34;
    m33._31_32_33 = mat._41_42_44;
    ret._32 = -determinant(m33);

	//24
    m33._11_12_13 = mat._11_12_13;
    m33._21_22_23 = mat._31_32_33;
    m33._31_32_33 = mat._41_42_43;
    ret._42 = determinant(m33);

	//31
    m33._11_12_13 = mat._12_13_14;
    m33._21_22_23 = mat._22_23_24;
    m33._31_32_33 = mat._42_43_44;
    ret._13 = determinant(m33);

	//32
    m33._11_12_13 = mat._11_13_14;
    m33._21_22_23 = mat._21_23_24;
    m33._31_32_33 = mat._41_43_44;
    ret._23 = -determinant(m33);

	//33
    m33._11_12_13 = mat._11_12_14;
    m33._21_22_23 = mat._21_22_24;
    m33._31_32_33 = mat._41_42_44;
    ret._33 = determinant(m33);

	//34
    m33._11_12_13 = mat._11_12_13;
    m33._21_22_23 = mat._21_22_23;
    m33._31_32_33 = mat._41_42_43;
    ret._43 = -determinant(m33);

	//41
    m33._11_12_13 = mat._12_13_14;
    m33._21_22_23 = mat._22_23_24;
    m33._31_32_33 = mat._32_33_34;
    ret._14 = -determinant(m33);

	//42
    m33._11_12_13 = mat._11_13_14;
    m33._21_22_23 = mat._21_23_24;
    m33._31_32_33 = mat._31_33_34;
    ret._24 = determinant(m33);

	//43
    m33._11_12_13 = mat._11_12_14;
    m33._21_22_23 = mat._21_22_24;
    m33._31_32_33 = mat._31_32_34;
    ret._34 = -determinant(m33);

	//44
    m33._11_12_13 = mat._11_12_13;
    m33._21_22_23 = mat._21_22_23;
    m33._31_32_33 = mat._31_32_33;
    ret._44 = determinant(m33);

    ret = ret / det;

    return ret;
}

matrix simpleinverse(matrix m)
{
    return transpose(m);
}

matrix identity()
{
    return matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

//��]����菜�����s����擾����
matrix ExcludeRotation(matrix mat)
{
    matrix rtn = identity();
    rtn._14_24_34 = mat._14_24_34;
    rtn._11 = sqrt(dot((mat._11_12_13), (mat._11_12_13)));
    rtn._22 = sqrt(dot((mat._21_22_23), (mat._21_22_23)));
    rtn._33 = sqrt(dot((mat._31_32_33), (mat._31_32_33)));
    return rtn;
}

//���s�ړ���������菜�����s����擾����
matrix ExcludeTranslation(matrix mat)
{
    matrix rtn = mat;
    rtn._14_24_34 = float3(0, 0, 0);
    return rtn;
}