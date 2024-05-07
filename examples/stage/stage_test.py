import sgl

from sgl.render import Stage, Xform, Light

stage = Stage()
xform = Xform(stage, "/root")
light = Light(stage, "/root/light")

print(stage)
print(xform)
print(light)
