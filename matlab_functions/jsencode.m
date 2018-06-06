function [] = jsencode(fid, var)
txt = jsonencode(var);
fprintf(fid, txt);
fclose(fid);
end