import voxical

slices = voxical.makeslice('models/yoda.stl')

print len(slices)
for sl in slices:
	for seg in sl:
		pass
		print seg[0].x
		print seg[0].y
		print seg[1].x
		print seg[1].y
