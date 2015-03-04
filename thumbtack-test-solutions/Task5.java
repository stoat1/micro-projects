/*
 * 5. Определить, сколько в целочисленной матрице MxN имеется различных 
 * элементов
 */

package thumbtacktasks;

import java.util.Set;
import java.util.TreeSet;

/**
 * @author Евгений Трапезников
 */
public class Task5
{       
    /**
     * The solution of the task
     * @param matrix - just matrix
     * @return count of different elems in matrix
     */
    public static int solution(int[][] matrix)
    {
        Set<Integer> elems = new TreeSet<Integer>();
        
        for(int[] row : matrix)
        {
            for(int elem : row)
            {
                elems.add(elem);
            }
        }
        
        return elems.size();
    }
}
