/*
 * Copyright (c) 2009-2012, Peter Abeles. All Rights Reserved.
 *
 * This file is part of Efficient Java Matrix Library (EJML).
 *
 * EJML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * EJML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with EJML.  If not, see <http://www.gnu.org/licenses/>.
 */

package org.ejml.example;

import org.ejml.data.DenseMatrix64F;
import org.ejml.ops.RandomMatrices;
import org.ejml.simple.SimpleBase;

import java.util.Random;


/**
 * Example of how to extend "SimpleMatrix" and add your own functionality.  In this case
 * two basic statistic operations are added.  Since SimpleBase is extended and StatisticsMatrix
 * is specified as the generics type, all "SimpleMatrix" operations return a matrix of
 * type StatisticsMatrix, ensuring strong typing.
 *
 * @author Peter Abeles
 */
public class StatisticsMatrix extends SimpleBase<StatisticsMatrix> {

    public StatisticsMatrix( int numRows , int numCols ) {
        super(numRows,numCols);
    }

    protected StatisticsMatrix(){}

    /**
     * Wraps a StatisticsMatrix around 'm'.  Does NOT create a copy of 'm' but saves a reference
     * to it.
     */
    public static StatisticsMatrix wrap( DenseMatrix64F m ) {
        StatisticsMatrix ret = new StatisticsMatrix();
        ret.mat = m;

        return ret;
    }

    /**
     * Computes the mean or average of all the elements.
     *
     * @return mean
     */
    public double mean() {
        double total = 0;

        final int N = getNumElements();
        for( int i = 0; i < N; i++ ) {
            total += get(i);
        }

        return total/N;
    }

    /**
     * Computes the unbiased standard deviation of all the elements.
     *
     * @return standard deviation
     */
    public double stdev() {
        double m = mean();

        double total = 0;

        final int N = getNumElements();
        if( N <= 1 )
            throw new IllegalArgumentException("There must be more than one element to compute stdev");


        for( int i = 0; i < N; i++ ) {
            double x = get(i);

            total += (x - m)*(x - m);
        }

        total /= (N-1);

        return Math.sqrt(total);
    }

    /**
     * Returns a matrix of StatisticsMatrix type so that SimpleMatrix functions create matrices
     * of the correct type.
     */
    @Override
    protected StatisticsMatrix createMatrix(int numRows, int numCols) {
        return new StatisticsMatrix(numRows,numCols);
    }

    public static void main( String args[] ) {
        Random rand = new Random(24234);

        int N = 500;

        // create two vectors whose elements are drawn from uniform distributions
        StatisticsMatrix A = StatisticsMatrix.wrap(RandomMatrices.createRandom(N,1,0,1,rand));
        StatisticsMatrix B = StatisticsMatrix.wrap(RandomMatrices.createRandom(N,1,1,2,rand));

        // the mean should be about 0.5
        System.out.println("Mean of A is               "+A.mean());
        // the mean should be about 1.5
        System.out.println("Mean of B is               "+B.mean());

        StatisticsMatrix C = A.plus(B);

        // the mean should be about 2.0
        System.out.println("Mean of C = A + B is       "+C.mean());

        System.out.println("Standard deviation of A is "+A.stdev());
        System.out.println("Standard deviation of B is "+B.stdev());
        System.out.println("Standard deviation of C is "+C.stdev());
    }
}
