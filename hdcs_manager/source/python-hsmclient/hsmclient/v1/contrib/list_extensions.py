
from hsmclient import base
from hsmclient import utils


class ListExtResource(base.Resource):
    @property
    def summary(self):
        descr = self.description.strip()
        if not descr:
            return '??'
        lines = descr.split("\n")
        if len(lines) == 1:
            return lines[0]
        else:
            return lines[0] + "..."


class ListExtManager(base.Manager):
    resource_class = ListExtResource

    def show_all(self):
        return self._list("/extensions", 'extensions')


@utils.service_type('hsm')
def do_list_extensions(client, _args):
    """
    List all the os-api extensions that are available.
    """
    extensions = client.list_extensions.show_all()
    fields = ["Name", "Summary", "Alias", "Updated"]
    utils.print_list(extensions, fields)
