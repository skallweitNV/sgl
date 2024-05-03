import sgl

d = sgl.Device()
p = d.load_program("test2.slang", ["main"], link_options={"dump_intermediates": True})
k = d.create_compute_kernel(p)
