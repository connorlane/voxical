#!/usr/bin/python

import sys, numpy, matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from stl import mesh 
import bisect
import time

def sliceTriangle(triangle, zHeight):
	if len(triangle) != 3 or len(triangle[0]) != 3:
		print "Invalid number of arguments to sliceTriangle"
		exit(-1)

	intersectionPoints = [[],[]]

	if triangle[0][2] < zHeight and triangle[1][2] >= zHeight:
		zSlice = zHeight - triangle[0][2]
		zDiff = triangle[2][2] - triangle[0][2]
		intersectionPoints[0] = [zSlice*(triangle[2][0] - triangle[0][0])/zDiff + triangle[0][0],
										 zSlice*(triangle[2][1] - triangle[0][1])/zDiff + triangle[0][1],
										 zSlice + triangle[0][2]]

		zDiff = triangle[1][2] - triangle[0][2]
		intersectionPoints[1] = [zSlice*(triangle[1][0] - triangle[0][0])/zDiff + triangle[0][0],
										 zSlice*(triangle[1][1] - triangle[0][1])/zDiff + triangle[0][1],
										 zSlice + triangle[0][2]]

	elif triangle[1][2] < zHeight and triangle[2][2] > zHeight:
		zSlice = zHeight - triangle[0][2]
		zDiff = triangle[2][2] - triangle[0][2]
		intersectionPoints[0] = [zSlice*(triangle[2][0] - triangle[0][0])/zDiff + triangle[0][0],
										 zSlice*(triangle[2][1] - triangle[0][1])/zDiff + triangle[0][1],
										 zSlice + triangle[0][2]]

		zSlice = zHeight - triangle[1][2]
		zDiff = triangle[2][2] - triangle[1][2]
		intersectionPoints[1] = [zSlice*(triangle[2][0] - triangle[1][0])/zDiff + triangle[1][0],
										 zSlice*(triangle[2][1] - triangle[1][1])/zDiff + triangle[1][1],
										 zSlice + triangle[1][2]] 
	#else:
	#	print "Received invalid triangle: " + str(triangle)
	
	return intersectionPoints

def plotTriangle(triangle, ax):
	ax.plot([triangle[0][0], triangle[1][0]], [triangle[0][1], triangle[1][1]], zs=[triangle[0][2], triangle[1][2]], color='red')
	ax.plot([triangle[1][0], triangle[2][0]], [triangle[1][1], triangle[2][1]], zs=[triangle[1][2], triangle[2][2]], color='red')
	ax.plot([triangle[2][0], triangle[0][0]], [triangle[2][1], triangle[0][1]], zs=[triangle[2][2], triangle[0][2]], color='red')

def plotLine(pairOfPoints, ax):
	ax.plot([pairOfPoints[0][0], pairOfPoints[1][0]], [pairOfPoints[0][1], pairOfPoints[1][1]], zs=[pairOfPoints[0][2], pairOfPoints[1][2]], color='blue')
	

#if len(sys.argv) != 10:
#	print "Invalid number of arguments"
#	exit(-1)

#triangle = [ [ float(sys.argv[1]), float(sys.argv[2]), float(sys.argv[3])],
#				 [ float(sys.argv[4]), float(sys.argv[5]), float(sys.argv[6])],
#				 [ float(sys.argv[7]), float(sys.argv[8]), float(sys.argv[9])] ]

mesh = mesh.Mesh.from_file('yoda.stl')

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

#print mesh.vectors[0]
#print mesh.vectors[:,2]

# Sort points in each triangle by z height
for i in range(0, len(mesh.vectors)):
	mesh.vectors[i] = mesh.vectors[i][mesh.vectors[i][:,2].argsort()]

# Sort all triangles by smallest z height
sortedTriangles = mesh.vectors[mesh.vectors[:,0,2].argsort()]
#sortedTriangles = sorted(mesh.vectors, key=lambda x: min(x[:,2]))

for triangle in sortedTriangles:
	triangle = triangle[triangle[:,2].argsort()]

print sortedTriangles

activeTriangleList = []
currentIndex = 0
sliceIndex = 0
	
while currentIndex < len(sortedTriangles):	
	zSlice = sliceIndex * 0.5
	
	print "Slicing layer #" + str(sliceIndex)

	print "...Removing old triangles"
	t = time.time()
	# Remove the triangles in the active list which aren't overlapping the z slice anymore	
	while activeTriangleList and activeTriangleList[0][2,2] < zSlice:
		activeTriangleList.pop(0)
	print "      Duration: " + str(time.time() - t)

	print "..Finding new triangles"
	t = time.time()
	# Continue where we last left off looking for triangles whose minimum z point is below the new z slice
	while currentIndex < len(sortedTriangles) and sortedTriangles[currentIndex,0,2] < zSlice:
		thisTriangle = sortedTriangles[currentIndex]
		zMax = thisTriangle[2,2]

		# If this triangle spans the current z slice...
		if zMax > zSlice:
			activeTriangleIndex = 0

			# Maintain sorted property of the active triangle list by inserting this triangle into the list
			#    after all the triangles with lower maximum z values and before all the triangles with higher
			#    maximum z values
			while activeTriangleIndex < len(activeTriangleList) and activeTriangleList[activeTriangleIndex][2,2] < zMax:
				activeTriangleIndex = activeTriangleIndex + 1
			activeTriangleList.insert(activeTriangleIndex, thisTriangle)	

		currentIndex = currentIndex + 1
	print "      Duration: " + str(time.time() - t)

	print "...Slicing triangles"
	t = time.time()
	# Slice all of the triangles in the active triangle list at the current z height
	for activeTriangle in activeTriangleList:
		slicedPoints = sliceTriangle(activeTriangle, zSlice)
		if slicedPoints == [[],[]]:
			print "Error: " + str(activeTriangle) + " at " + str(zSlice)
		plotLine(slicedPoints, ax)
	print "      Duration: " + str(time.time() - t)

	# Increment sliceIndex
	sliceIndex = sliceIndex + 1

#for triangle in lowestTriangles[:1000]:
#

#
#for line in intersectionVerteces:
#		ax.plot([line[0][0], line[1][0]], [line[0][1], line[1][1]], zs=[line[0][2], line[1][2]], color='blue')
#

plt.show()
Axes3D.plot()
