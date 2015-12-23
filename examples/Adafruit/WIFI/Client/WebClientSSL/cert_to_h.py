import sys

if (len(sys.argv) < 2):
  print "Wrong syntax!"
  #print "Usage: python certs_to_h.py <cert_file> <output_variable_name> <output_file_name>"
  print "Usage: python certs_to_h.py <cert_file>"
  sys.exit()

f_in = open(sys.argv[1], 'r')
f_out = open("certificates.h", 'w')
#f_out = open(sys.argv[3]+".h", 'w'# )

variable_name = "root_certs"

f_out.write(
"#ifndef _CERTIFICATE_H_\n\
#define _CERTIFICATE_H_\n\
\n\
#ifdef __cplusplus\n\
extern \"C\"\n\
{\n\
#endif\n\
\n")
             
f_out.write("const char ")
f_out.write(variable_name)
f_out.write("[] =")

for line in f_in:
    line = line.strip()
    f_out.write("\n\"")
    f_out.write(line)
    f_out.write("\\r\\n\"")

f_out.write(";\n")
f_out.write(
"\n\
#ifdef __cplusplus\n\
} /* extern \"C\" */\n\
#endif\n\
\n\
#endif /* ifndef _CERTIFICATE_H_ */")
                 
f_in.close()
f_out.close()
